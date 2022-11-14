//
// Created by User on 11/13/2022.
//

#ifndef ECS_EITHER_H
#define ECS_EITHER_H

template <class TL, class TR>
class either {
private:
    union {
        TL left_;
        TR right_;
    };
    bool is_left_{};
public:
    explicit either(TL left) : left_(left), is_left_(true) {}
    explicit either(TR right) : right_(right), is_left_(false) {}

    template<typename T>
    T match(T (*left)(TL), T (*right)(TR)) {
        return is_left_ ? left(left_) : right(right_);
    }

    ~either() {
        if (is_left_) left_.~TL();
        else right_.~TR();
    }
};

#endif //ECS_EITHER_H
