#include <iostream>
#include <limits>
#include <vector>

#include <algorithm>
#include <functional>
#include <cassert>

#define OPEN true
#define CLOSE false

#define INF std::numeric_limits<float>::infinity()

class Interval {

public:
    Interval() {
        /* left and right bounds are set to -inf and +inf */
        left = -INF;
        right = INF;
        /* default interval state is close */
        left_type = OPEN;
        right_type = OPEN;
    }

    Interval(float l, float r, bool lt, bool rt):
        left(l), right(r), left_type(lt), right_type(rt) {}

    float get_left() const { return left; }
    float get_left_type() const { return left_type; }
    float get_right() const { return right; }
    float get_right_type() const { return right_type; }

    /* true if empty, false otherwise. */
    bool is_empty() const {
        return (left == right) ? ( (left_type == OPEN) || (right_type == OPEN) ) : (left > right);
    }

    /* true if intervals are equals, fasle otherwise */
    bool is_equal(const Interval& intv) const {
        return (left == intv.get_left()) && (right == intv.get_right()) && (left_type == intv.get_left_type()) && (right_type == intv.get_right_type());
    }

    void print(bool el = true) const {
        if (left == -INF && right== INF)
            std::cout << "";
        else
            std::cout << ((left_type) ? "(" : "[") << left << ", " << right << ((right_type) ? ")" : "]");

        if (el)
            std::cout << std::endl;
    }

    /* intersection between intervals */
    Interval intersection(const Interval& intv_right) const {
        float n_left, n_right;
        bool n_ltype, n_rtype;

        if (left > intv_right.get_left()) {
            n_left = left;
            n_ltype = left_type;
        } else if (left < intv_right.get_left()) {
            n_left = intv_right.get_left();
            n_ltype = intv_right.get_left_type();
        } else {
            n_left = left;
            n_ltype = (left_type == OPEN) ? OPEN : intv_right.get_left_type();
        }

        if (right < intv_right.get_right()) {
            n_right = right;
            n_rtype = right_type;
        } else if (right > intv_right.get_right()) {
            n_right = intv_right.get_right();
            n_rtype = intv_right.get_right_type();
        } else {
            n_right = right;
            n_rtype = (right_type == OPEN) ? OPEN : intv_right.get_right_type();
        }

        return Interval(n_left, n_right, n_ltype, n_rtype);
    }

    /*subtract intv_right from this*/
    std::vector<Interval> setminus(const Interval& intv_right, bool split=false) const {
        std::vector<Interval> output;
        Interval inter = this->intersection(intv_right);

        if (inter.is_empty()) {
            output.emplace_back(left, right, left_type, right_type);
            return output;
        }

        if (this->is_equal(inter)) {
            if (split)
                output.push_back(inter);
            return output;
        }

        if (left == inter.get_left()) {
            if (left == inter.get_right())
                output.emplace_back(left, right, !inter.get_right_type(), right_type);
            else {
                if (inter.get_left_type() == OPEN && left_type == CLOSE)
                    output.emplace_back(left, left, left_type, left_type);

                output.emplace_back(inter.get_right(), right, !inter.get_right_type(), right_type);
            }
        } else if (right == inter.get_right()) {
            if (right == inter.get_left())
                output.emplace_back(left, right, left_type, !inter.get_right_type());
            else {
                if (inter.get_right_type() == OPEN && right_type == CLOSE)
                    output.emplace_back(right, right, right_type, right_type);

                output.emplace_back(left, inter.get_left(), left_type, !inter.get_left_type());
            }
        } else {
            output.emplace_back(left, inter.get_left(), left_type, !inter.get_left_type());
            output.emplace_back(inter.get_right(), right, !inter.get_right_type(), right_type);
        }

        if (split)
            output.push_back(inter);

        return output;
    }

private:
    float left, right;
    bool left_type, right_type;
};



/* intervals operations */
bool operator==(const Interval &l, const Interval &r) {
    return (l.get_left() == r.get_left()) && (l.get_right() == r.get_right()) && (l.get_left_type() == r.get_left_type()) && (l.get_right_type() == r.get_right_type());
}

bool operator!=(const Interval &l, const Interval &r) {
    return !(l == r);
}

Interval operator+(const Interval& l, const Interval& r) {
    return Interval(l.get_left() + r.get_left(), l.get_right() + r.get_right(), (l.get_left_type() || r.get_left_type()), (l.get_right_type() || r.get_right_type()));
}

Interval operator+(const size_t v, const Interval& i) {
    return Interval(v + i.get_left(), v + i.get_right(), i.get_left_type(), i.get_right_type());
}

Interval operator+(const float v, const Interval& i) {
    return Interval(v + i.get_left(), v + i.get_right(), i.get_left_type(), i.get_right_type());
}

Interval operator*(const float v, const Interval& i) {
    return Interval(v * i.get_left(), v * i.get_right(), i.get_left_type(), i.get_right_type());
}

bool operator>(const float v, const Interval& i) {
    if ((v > i.get_left()) || (v == i.get_left() && i.get_left_type() == CLOSE))
        if ((v < i.get_right()) || (v == i.get_right() && i.get_right_type() == CLOSE))
            return true;

    return false;
}

bool operator<(const Interval &l, const Interval &r) {
    if (l.get_left() <= r.get_left()) {
        if (l.get_left() == r.get_left()) {
            if (l.get_left_type() == CLOSE && r.get_left_type() == OPEN)
                return true;

            if (l.get_left_type() == OPEN && r.get_left_type() == CLOSE)
                return false;
            
            if (l.get_right() <= r.get_right()) {
                if (l.get_right() == r.get_right()) {
                    if (l.get_right_type() == CLOSE && r.get_right_type() == OPEN)
                        return false;

                    if (l.get_right_type() == OPEN && r.get_right_type() == CLOSE)
                        return true;
                    
                    return false;
                }
                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}



class HyperRectangle {

public:
    HyperRectangle() {
        dimension = 0;
        intervals = std::vector<Interval>(dimension);
    }

    HyperRectangle(const std::vector<Interval>& intvs) {
        dimension = intvs.size();
        intervals = intvs;
    }

    HyperRectangle(size_t dim, bool empty=false) {
        dimension = dim;
        if (!empty)
            intervals = std::vector<Interval>(dimension);
        else {
            for (size_t i = 0; i < dimension; ++i) {
                intervals.emplace_back(0, 0, 1, 1);
            }
        }
    }
    
    HyperRectangle(size_t dim, double noise, size_t closed_interval) {
        dimension = dim;
        for (size_t i = 0; i < dimension; ++i) {
            intervals.emplace_back(-noise, noise, closed_interval, closed_interval);
        }
        
    }

    bool is_empty() const {
        for (const Interval &i : intervals)
            if (i.is_empty())
                return true;
        return false;
    }

    bool is_equal(const HyperRectangle &r) const {
        for (size_t i = 0; i < r.get_dim(); ++i)
            if(!(intervals[i] == r.get_intvs_ref()[i]))
                return false;

        return true;
    }

    double get_pert() { 
        return intervals[0].get_right();
    }

    std::vector<Interval> get_intvs() const {
        return intervals;
    }

    const std::vector<Interval>& get_intvs_ref() const {
        return intervals;
    }

    size_t get_dim() const {
        return dimension;
    }

    Interval& operator[](const size_t i) {
        return intervals[i];
    }

    void print(bool el=true) const {
        std::cout << "<";
        for (size_t i = 0; i < dimension; ++i) {
            intervals[i].print(false);
            if (i < dimension - 1)
                std::cout << ", ";
        }
        std::cout << ">";

        if (el)
            std::cout << std::endl;
    }

    std::vector<HyperRectangle> _setminus_aux(const HyperRectangle& intv, size_t dim) const {
        std::vector<Interval> intv_list = intervals[dim].setminus(intv.get_intvs()[dim], true);
        std::vector<HyperRectangle> hrect_list;
        std::vector<Interval> changed;

        for (int i = 0; i < intv_list.size(); ++i) {
            if (!intv_list[i].is_empty()) {
                changed = intervals;
                changed[dim] = intv_list[i];
                hrect_list.emplace_back(changed);
            }
        }

        return hrect_list;
    }

    std::vector<HyperRectangle> _setminus(const HyperRectangle& hype, size_t dim) const {
        std::vector<HyperRectangle> hrect_list;

        /* return an empty vector */
        if (dim >= dimension)
            return hrect_list;

        std::vector<HyperRectangle> current = this->_setminus_aux(hype, dim);
        for (int i = 0; i < current.size() - 1; ++i)
            hrect_list.push_back(current[i]);

        std::vector<HyperRectangle> set_minus = current[current.size() - 1]._setminus(hype, dim + 1);
        hrect_list.insert(std::end(hrect_list), set_minus.begin(), set_minus.end());
        return hrect_list;
    }

    std::vector<HyperRectangle> setminus(const HyperRectangle& hype) const {
        std::vector<HyperRectangle> output;
        HyperRectangle inter = this->intersection(hype);

        if (this->is_equal(hype))
            return std::vector<HyperRectangle>();
            
        
        if (inter.is_empty()) {
            output.push_back(*this);
            return output;
        }

        return _setminus(hype, 0);
    }

    HyperRectangle intersection(const HyperRectangle& hype) const {
        std::vector<Interval> intv;
        const std::vector<Interval>& intv_right = hype.get_intvs_ref();

        for (size_t i = 0; i < dimension; ++i) {
            intv.push_back(intervals[i].intersection(intv_right[i]));
        }

        return intv;
    }

private:
    size_t dimension;
    std::vector<Interval> intervals;
};



/* hyperrectangle operations */
bool operator==(const HyperRectangle &l, const HyperRectangle &r) {
    for (size_t i = 0; i < l.get_dim(); ++i)
        if(!(l.get_intvs_ref()[i] == r.get_intvs_ref()[i]))
            return false;

    return true;
}

bool operator!=(const HyperRectangle &l, const HyperRectangle &r) {
    return !(l==r);
}

bool operator<(const HyperRectangle& l, const HyperRectangle& r) {
    for (size_t i = 0; i < l.get_dim(); ++i) {
        if(l.get_intvs_ref()[i] < r.get_intvs_ref()[i])
            return true;
        if(r.get_intvs_ref()[i] < l.get_intvs_ref()[i])
            return false;
    }

    return false;
}

size_t operator>(const std::vector<float> ist, const HyperRectangle& h) {
    for (size_t i = 0; i < h.get_dim(); ++i) {
        if (!(ist[i] > const_cast<HyperRectangle&>(h)[i]))
            return false;
    }
    return true;
}

HyperRectangle operator+(const HyperRectangle& l, const HyperRectangle& r) {
    assert(l.get_dim() == r.get_dim());

    std::vector<Interval> sum;
    sum.reserve(r.get_dim());

    std::transform(l.get_intvs_ref().begin(), l.get_intvs_ref().end(), r.get_intvs_ref().begin(), std::back_inserter(sum), std::plus<Interval>());
    return HyperRectangle(sum);
}

HyperRectangle operator+(const std::vector<float> v, const HyperRectangle& h) {
    std::vector<Interval> out;
    const std::vector<Interval>& intv = h.get_intvs_ref();
    for (size_t i = 0; i < v.size(); ++i) {
        out.push_back(v[i] + intv[i]);
    }
 
    return out;
}

HyperRectangle operator*(const std::vector<float> v, const HyperRectangle& h) {
    std::vector<Interval> out;
    const std::vector<Interval>& intv = h.get_intvs_ref();
    for (size_t i = 0; i < v.size(); ++i) {
        out.push_back(v[i] * intv[i]);
    }
 
    return out;
}

HyperRectangle operator+(float v, const HyperRectangle& h) {
    std::vector<Interval> out;
    const std::vector<Interval>& intv = h.get_intvs_ref();
    for (size_t i = 0; i < h.get_dim(); ++i) {
        out.push_back(v + intv[i]);
    }
 
    return out;
}

std::vector<HyperRectangle> operator-(const HyperRectangle& l, const HyperRectangle& r) {
    return l.setminus(r);
}
