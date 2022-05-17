template <typename _Tp>
RingBuffer<_Tp>::RingBuffer(){
   _M_capacity = RING_BUFFER_INIT_SIZE;
   _M_read_pos = _M_write_pos = 0;
   _M_content = (_Tp*)calloc(RING_BUFFER_INIT_SIZE,sizeof(_Tp));
}
/* Destroy the given ring buffer, free all resources you've allocated. */
template <typename _Tp>
RingBuffer<_Tp>::~RingBuffer(){
   if(_M_content == nullptr) return;
   free(_M_content);
   _M_content = nullptr;
}
template <typename _Tp>
size_t RingBuffer<_Tp>::get_capacity() const{
      return _M_capacity;
   }

/* Returns true if the given ring buffer is empty, false otherwise */
template <typename _Tp>
bool RingBuffer<_Tp>::is_empty() const{
   if(_M_content == NULL) return false;
   return (_M_read_pos == _M_write_pos);
}

/* Read an element from the ring buffer (i.e., the dequeue operation), *data records 
the element popped. returns true if the operation succeeded, false otherwise. */
template <typename _Tp>
bool RingBuffer<_Tp>::read(_Tp& data){// the same to read_mult
   size_t rdsize = 1;
   size_t num = 0;
   if(_M_content == nullptr) return false;/*whether input is NULL*/
   // if(data.size() == 0) return false;/*whether input is NULL*/
   num = _M_write_pos - _M_read_pos;/*count the num of number in buffer */
   if (num < rdsize) return false;/*not enough*/
   else
   {
      size_t i = 0;
      while (i < rdsize)
      {
         data.push_back(_M_content[(_M_read_pos++)%_M_capacity]);/*write to data and update index and read_pos(just like pop)*/
         i++;
      }
   }
   return true;/*if we finish the part return true*/
}

/* Write an element which value is `data` to the ring buffer (i.e., the enqueue operation),
returns true if the operation succeeded, false otherwise. Note that when the buffer is 
full, grow its size to make sure the element can be write successfully. The detailed 
requirements of size growth are in the homework description. */
template <typename _Tp>
bool RingBuffer<_Tp>::write(const _Tp& data){
   if(_M_content == nullptr) return false;/*whether input is NULL*/
   if (!(_M_write_pos-_M_read_pos==_M_capacity)) /*just judge whether the buffer is full*/
   {
      _M_content[((_M_write_pos)++)%_M_capacity] = data; /*write and update write_pos*/
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
      tmp = (_Tp *)calloc(_M_capacity, sizeof(_Tp));/*calloc memory to update content*/
      if (_M_read_pos%cap < _M_write_pos%cap)
         memcpy(tmp, _M_content, cap * sizeof(_Tp));
      else/*in this case we should cp the memory in two times to resort them*/
      {
         memcpy(tmp, _M_content + _M_read_pos%cap, (cap - (_M_read_pos%cap)) * sizeof(_Tp));
         memcpy(tmp + cap - _M_read_pos%cap, _M_content, (_M_read_pos%cap) * sizeof(_Tp));
      }
      _M_read_pos = 0;/*reinit the read_pos*/
      _M_write_pos = cap;/*reinit the write_pos*/
      tmp[((_M_write_pos)++)%_M_capacity] = data;/*write and update write_pos*/
      free(_M_content);/*free old content*/
      _M_content = tmp;/*update*/
      return true;
   }
   return false;
}

/* Read "rdsize" elements from the ring buffer, and fill them to "data".
Notice that you should only return true when this operation succeeds.
If there are not enough elements in the buffer, just DO NOTHING and 
return false. DO NOT modify the ring buffer and data for such cases.
If size of "data" vector does not match "rdsize", you should resize "data" */
template <typename _Tp>
bool RingBuffer<_Tp>::read_multi(size_t rdsize, std::vector<_Tp>& data){
   size_t num = 0;
   if(_M_content == nullptr) return false;/*whether input is NULL*/
   // if(data.size() == 0) return false;/*whether input is NULL*/
   num = _M_write_pos - _M_read_pos;/*count the num of number in buffer */
   if (num < rdsize) return false;/*not enough*/
   else
   {
      size_t i = 0;
      while (i < rdsize)
      {
         data.push_back(_M_content[(_M_read_pos++)%_M_capacity]);/*write to data and update index and read_pos(just like pop)*/
         i++;
      }
   }
   return true;/*if we finish the part return true*/
   
}

/* Write "wrtsize" elements to the ring buffer. The values to write are 
provided in "data". Returns true if succeeds, false otherwise. Don't 
forget to grow your buffer size properly. */
template <typename _Tp>
bool RingBuffer<_Tp>::write_multi(size_t wrtsize, const std::vector<_Tp>& data){
   size_t i = 0;
   if(data.size() == 0) return false;/*whether input is NULL*/
   while (i < wrtsize)/*use while instead of for to avoid some warning*/
   {
      write(data[i]);/*ring_buffer_write function to write one by one*/
      i++;
   }
   return true;
}

/* For every element in the ring buffer, apply "func" to it. For example,
Your buffer currently contains 3 elements 1, 2, 4. If the map function is to 
"add five", then their values should be 6, 7, 9 after calling this function. */
template <typename _Tp>
bool RingBuffer<_Tp>::map(std::function<_Tp(_Tp)>&& func){
   size_t index = _M_read_pos%_M_capacity;
   if(_M_content == nullptr) return false;/*whether input is NULL*/
   if (_M_read_pos < _M_write_pos)/*calculate it in two case*/
   {
      while (index < _M_write_pos%_M_capacity)
      {
         _M_content[index] = func(_M_content[index]);
         index++;/*to avoid ubs we cannot write the two lines in two lines*/
      }
      return true;/*if we finish the part return true*/
   }
   else /*calculate it in two case*/
   {
      while (index <= _M_capacity && index > _M_write_pos%_M_capacity)
      {
         _M_content[index] = func(_M_content[index]);
         index++;/*to avoid ubs we cannot write the two lines in two lines*/
         if (index == _M_capacity && index > _M_write_pos%_M_capacity)
         {
            index = 0;/*restart from the begin*/
            while (index < _M_write_pos%_M_capacity)
            {
               _M_content[index] = func(_M_content[index]);
               index++;/*to avoid ubs we cannot write the two lines in two lines*/
            }
         }
         break;/*to avoid some other problem*/
      }
      return true;/*if we finish the part return true*/
   }
   return false;/*if we cannot return before there must be some error*/
}
template <typename _Tp>
void RingBuffer<_Tp>::grow(){
      if (_M_capacity < 1024) /*the two case of grow*/
            _M_capacity *= RING_BUFFER_GROW_FACTOR1;
         else
            _M_capacity *= RING_BUFFER_GROW_FACTOR2;
   }

template<typename _Tp>
class __detail::__iterator{
public: 
   //constructor
   __iterator():_ptr(nullptr),_buffer(nullptr){}

   __iterator(const __iterator& other) {
      _ptr = other._ptr;
      _buffer = nullptr;
   }

   __iterator(_Tp *ptr,RingBuffer<_Tp> * buffer): _ptr(ptr),_buffer(buffer) {
      _index = _ptr - _buffer->_M_content;
   }

   ~__iterator() = default;

   __iterator operator+(int n){
      return __iterator(&_buffer->_M_content[(_index+n)%_buffer->_M_capacity],_buffer);
   }
   __iterator operator-(int n){
      return __iterator(&_buffer->_M_content[(_index-n)%_buffer->_M_capacity],_buffer);
   }
   
   
   __iterator& operator=(const __iterator & x){
      _ptr = x._ptr;
      _index = x._index;
      _buffer = x._buffer;
      return *this;
   }

    __iterator<_Tp>& operator+=(int n) {
        index += n;
        _ptr = _buffer->_M_content+(_index)%_buffer->_M_capacity;
        return *this;
    }

    __iterator<_Tp>& operator-=(int n) {
        index -= n;
        _ptr = _buffer->_M_content+(_index)%_buffer->_M_capacity;
        return *this;
    }

   // ++i
   __iterator operator++()  
   {
      _index++;
      _ptr = _buffer->_M_content+(_index)%_buffer->_M_capacity;
      return *this;
   }
   // i++
   __iterator operator++(int)         
   {
      __iterator Copy(_ptr,_buffer);
      _index++;
      _ptr = _buffer->_M_content+(_index)%_buffer->_M_capacity;
      return Copy; 
   }
   // --i
   __iterator operator--()  
   {
      _index--;
      _ptr = _buffer->_M_content+(_index)%_buffer->_M_capacity;
      return *this;
   }
   // i--
   __iterator operator--(int)         
   {
      __iterator Copy(_ptr,_buffer);
      _index--;
      _ptr = _buffer->_M_content+(_index)%_buffer->_M_capacity;
      return Copy; 
   }
   
   _Tp& operator*() {
      return *_ptr;
    }

   _Tp* operator->() {
      return _ptr;
   }
   
   bool operator==(const __iterator& x) {
      return _ptr == x._ptr;
    }
   
   bool operator!=(const __iterator& x) {
      return _ptr != x._ptr;
   }
   
   //data pointer
   _Tp* _ptr;
   RingBuffer<_Tp> * _buffer;
   size_t _index;
};