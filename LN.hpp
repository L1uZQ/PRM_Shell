#ifndef LN_HPP_
#define LN_HPP_

template<class T>
class LN {
public:
	LN() : next(nullptr) {}
	LN(const LN& ln) : value(ln.value), next(ln.next) {}
	LN(T v, LN* n = nullptr) : value(v), next(n) {}
	T value;
	LN* next;
};

#endif /* LN_HPP_ */
