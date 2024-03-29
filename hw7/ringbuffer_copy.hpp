#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cstdlib>
#include <cstring>

/* Initial size of the buffer */
#define RING_BUFFER_INIT_SIZE 8

/* You should grow buffer size to RING_BUFFER_GROW_FACTOR1 * _M_capacity, when current _M_capacity < 1024
   And RING_BUFFER_GROW_FACTOR2 * _M_capacity, when current _M_capacity >= 1024 */
#define RING_BUFFER_GROW_FACTOR1 2
#define RING_BUFFER_GROW_FACTOR2 1.5

namespace __detail
{
   // Iterator of vector (implement this)
   template <typename _Tp>
   class __iterator;

   // Const iterator of vector (implement this)
   template <typename _Tp>
   class __const_iterator;
}

/* The ring_buffer structure */
template <typename _Tp>
class RingBuffer
{
public:
   // Type renaming
   typedef size_t size_type;
   typedef _Tp value_type;
   typedef _Tp *pointer;
   typedef _Tp &reference;
   typedef const _Tp &const_reference;
   typedef ptrdiff_t difference_type;
   typedef typename __detail::__iterator<_Tp> iterator;
   typedef typename __detail::__const_iterator<_Tp> const_iterator;

   /* Create a new ring buffer. You should allocate memory for its _M_content,
      initialize the _M_read_pos, _M_write_pos, and its _M_capacity to RING_BUFFER_INIT_SIZE */
   RingBuffer();
   // Copy and move constructor are deleted
   RingBuffer(const RingBuffer &other) = delete;

   RingBuffer(RingBuffer &&other) = delete;

   /* Destroy the given ring buffer, free all resources you've allocated. */
   ~RingBuffer();
   /* Returns its _M_capacity */
   size_t get_capacity() const
   {
      return _M_capacity;
   }

   /* Returns true if the given ring buffer is empty, false otherwise */
   bool is_empty() const
   {
      if (_M_content == NULL)
         return false;
      return (_M_read_pos == _M_write_pos);
   }

   /* Read an element from the ring buffer (i.e., the dequeue operation), *data records
   the element popped. returns true if the operation succeeded, false otherwise. */
   bool read(_Tp &data)
   { // the same to read_mult
      size_t rdsize = 1;
      size_t num = 0;
      if (_M_content == nullptr)
         return false; /*whether input is NULL*/
      // if(data.size() == 0) return false;/*whether input is NULL*/
      num = _M_write_pos - _M_read_pos; /*count the num of number in buffer */
      if (num < rdsize)
         return false; /*not enough*/
      else
      {
         size_t i = 0;
         while (i < rdsize)
         {
            data.push_back(_M_content[(_M_read_pos++) % _M_capacity]); /*write to data and update index and read_pos(just like pop)*/
            i++;
         }
      }
      return true; /*if we finish the part return true*/
   }

   /* Write an element which value is `data` to the ring buffer (i.e., the enqueue operation),
   returns true if the operation succeeded, false otherwise. Note that when the buffer is
   full, grow its size to make sure the element can be write successfully. The detailed
   requirements of size growth are in the homework description. */
   bool write(const _Tp &data)
   {
      if (_M_content == nullptr)
         return false;                                  /*whether input is NULL*/
      if (!(_M_write_pos - _M_read_pos == _M_capacity)) /*just judge whether the buffer is full*/
      {
         _M_content[((_M_write_pos)++) % _M_capacity] = data; /*write and update write_pos*/
         return true;
      }
      else /*the buffer is full*/
      {
         size_t cap = _M_capacity;
         _Tp *tmp;
         if (_M_capacity < 1024) /*the two case of grow*/
            _M_capacity *= RING_BUFFER_GROW_FACTOR1;
         else
            _M_capacity *= RING_BUFFER_GROW_FACTOR2;
         tmp = (_Tp *)calloc(_M_capacity, sizeof(_Tp)); /*calloc memory to update content*/
         if (_M_read_pos % cap < _M_write_pos % cap)
            memcpy(tmp, _M_content, cap * sizeof(_Tp));
         else /*in this case we should cp the memory in two times to resort them*/
         {
            memcpy(tmp, _M_content + _M_read_pos % cap, (cap - (_M_read_pos % cap)) * sizeof(_Tp));
            memcpy(tmp + cap - _M_read_pos % cap, _M_content, (_M_read_pos % cap) * sizeof(_Tp));
         }
         _M_read_pos = 0;                              /*reinit the read_pos*/
         _M_write_pos = cap;                           /*reinit the write_pos*/
         tmp[((_M_write_pos)++) % _M_capacity] = data; /*write and update write_pos*/
         free(_M_content);                             /*free old content*/
         _M_content = tmp;                             /*update*/
         return true;
      }
      return false;
   }

   /* Read "rdsize" elements from the ring buffer, and fill them to "data".
   Notice that you should only return true when this operation succeeds.
   If there are not enough elements in the buffer, just DO NOTHING and
   return false. DO NOT modify the ring buffer and data for such cases.
   If size of "data" vector does not match "rdsize", you should resize "data" */
   bool read_multi(size_t rdsize, std::vector<_Tp> &data)
   {
      size_t num = 0;
      if (_M_content == nullptr)
         return false; /*whether input is NULL*/
      // if(data.size() == 0) return false;/*whether input is NULL*/
      num = _M_write_pos - _M_read_pos; /*count the num of number in buffer */
      if (num < rdsize)
         return false; /*not enough*/
      else
      {
         size_t i = 0;
         while (i < rdsize)
         {
            data.push_back(_M_content[(_M_read_pos++) % _M_capacity]); /*write to data and update index and read_pos(just like pop)*/
            i++;
         }
      }
      return true; /*if we finish the part return true*/
   }

   /* Write "wrtsize" elements to the ring buffer. The values to write are
   provided in "data". Returns true if succeeds, false otherwise. Don't
   forget to grow your buffer size properly. */
   bool write_multi(size_t wrtsize, const std::vector<_Tp> &data)
   {
      size_t i = 0;
      if (data.size() == 0)
         return false;    /*whether input is NULL*/
      while (i < wrtsize) /*use while instead of for to avoid some warning*/
      {
         write(data[i]); /*ring_buffer_write function to write one by one*/
         i++;
      }
      return true;
   }

   /* For every element in the ring buffer, apply "func" to it. For example,
   Your buffer currently contains 3 elements 1, 2, 4. If the map function is to
   "add five", then their values should be 6, 7, 9 after calling this function. */
   bool map(std::function<_Tp(_Tp)> &&func)
   {
      size_t index = _M_read_pos % _M_capacity;
      if (_M_content == nullptr)
         return false;                /*whether input is NULL*/
      if (_M_read_pos < _M_write_pos) /*calculate it in two case*/
      {
         while (index < _M_write_pos % _M_capacity)
         {
            _M_content[index] = func(_M_content[index]);
            index++; /*to avoid ubs we cannot write the two lines in two lines*/
         }
         return true; /*if we finish the part return true*/
      }
      else /*calculate it in two case*/
      {
         while (index <= _M_capacity && index > _M_write_pos % _M_capacity)
         {
            _M_content[index] = func(_M_content[index]);
            index++; /*to avoid ubs we cannot write the two lines in two lines*/
            if (index == _M_capacity && index > _M_write_pos % _M_capacity)
            {
               index = 0; /*restart from the begin*/
               while (index < _M_write_pos % _M_capacity)
               {
                  _M_content[index] = func(_M_content[index]);
                  index++; /*to avoid ubs we cannot write the two lines in two lines*/
               }
            }
            break; /*to avoid some other problem*/
         }
         return true; /*if we finish the part return true*/
      }
      return false; /*if we cannot return before there must be some error*/
   }

   /* The iterator is used to interate through the ringbuffer.
   By iterating through the ring buffer, it means start from the oldest element in the ring buffer
   (i.e. the element pointed by "_M_read_pos"),
   to the latest one. end() should return the slot after the last element.
   You can refer to standart library for more info  */
iterator begin()
{
   return iterator(_M_content + _M_read_pos % _M_capacity, this);
}
iterator end()
{
   return iterator(_M_content + _M_write_pos % _M_capacity, this);
}

// Const iterators
const_iterator cbegin() const
{
   return const_iterator(_M_content + _M_read_pos % _M_capacity, this);
}
const_iterator cend() const
{
   return const_iterator(_M_content + _M_write_pos % _M_capacity, this);
}

private:
   /* Grows the ringbuffer _M_capacity according to the growth factor */
   void grow();
   friend class __detail::__iterator<_Tp>;
   friend class __detail::__const_iterator<_Tp>;

private:
   size_t _M_capacity;
   size_t _M_read_pos;
   size_t _M_write_pos;
   _Tp *_M_content;
};

#include "ringbuffer.ipp"

#endif
