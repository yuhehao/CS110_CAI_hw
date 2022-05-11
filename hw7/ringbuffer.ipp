template <typename _Tp>
RingBuffer<_Tp>::iterator begin(){

}

template <typename _Tp>
RingBuffer<_Tp>::iterator end(){
    
}
template <typename _Tp>
RingBuffer<_Tp>:: const_iterator cbegin() {//const{
    
}

template <typename _Tp>
RingBuffer<_Tp>::const_iterator cend() {//const{

}
// RingBuffer
template <typename _Tp>
RingBuffer<_Tp>::RingBuffer(){
}

template <typename _Tp>
RingBuffer<_Tp>::~RingBuffer()
{
}

template <typename _Tp>
size_t RingBuffer<_Tp>::get_capacity() const
{
}

template <typename _Tp>
bool RingBuffer<_Tp>::is_empty() const
{
}

template <typename _Tp>
bool RingBuffer<_Tp>::read(_Tp &data)
{
}

template <typename _Tp>
bool RingBuffer<_Tp>::write(const _Tp &data)
{
}

template <typename _Tp>
bool RingBuffer<_Tp>::read_multi(size_t rdsize, std::vector<_Tp> &data)
{
}

template <typename _Tp>
bool RingBuffer<_Tp>::write_multi(size_t wrtsize, const std::vector<_Tp> &data)
{
}

template <typename _Tp>
bool RingBuffer<_Tp>::map(std::function<_Tp(_Tp)> &&func)
{
}

template <typename _Tp>
void RingBuffer<_Tp>::grow(){
    
}