#ifndef _RANGE_H_
#define _RANGE_H_

namespace ext {

  template <typename T = int>
  class range
  {
  public:
    class iterator
    {
    public:
      iterator(range<T> rng, T val) :
        _range(rng),
        _val(val)
      {}

      T    operator* () const                       { return _val; }
      bool operator!= (const iterator& other) const { return _val != other._val; }

      iterator& operator++() {
        if (_range.is_reverse()) { _val--; }
        else                     { _val++; }

        return *this;
      }

    private:
      range<T> _range;
      T        _val;
    };

    explicit range(T end, bool inclusive=false) :
      _begin(0),
      _end(end),
      _inclusive(inclusive)
    {
    }

    range(T begin, T end, bool inclusive=false) :
      _begin(begin),
      _end(end),
      _inclusive(inclusive)
    {
    }

    iterator begin() const {
      return iterator(*this, _begin);
    }

    iterator end() const {
      if (is_inclusive()) {
        return iterator(*this, is_reverse() ? _end - 1 : _end + 1);
      }
      else {
        return iterator(*this, _end);
      }
    }

    size_t size() const {
      size_t size = is_reverse() ? _begin - _end : _end - _begin;

      if (is_inclusive()) {
        size++;
      }

      return size;
    }

    size_t steps() const {
      return size() - 1;
    }

    bool is_reverse()   const { return _begin > _end; }
    bool is_inclusive() const { return _inclusive; }

  private:

    T    _begin;
    T    _end;
    bool _inclusive;
  };
}


#endif
