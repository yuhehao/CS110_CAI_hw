#include "ringbuffer.h"
/* Create a new ring buffer. You should allocate memory for its content,
   initialize the read_pos, write_pos and capacity properly.
   And finally return a pointer to the new ring buffer. */
ring_buffer *ring_buffer_new() /*use calloc instead of molloc to init*/
{
   /*
   // if write_pos = read_pos - 1 it is full.
   // if write_pos ==read_pos it is empty.*/
   ring_buffer *a = (ring_buffer *)calloc(1, sizeof(ring_buffer));
   a->capacity = RING_BUFFER_INIT_SIZE;
   a->read_pos = a->write_pos = 0; /*init read_pos and write_pos equal to 0*/
   a->content = (int *)calloc(RING_BUFFER_INIT_SIZE, sizeof(int));
   return a;
}
/* Destroy the given ring buffer, free all resources you've allocated.
   Finally you should set the buffer pointer to NULL, that is why we pass
   ring_buffer** as parameter, instead of ring_buffer*. */
void ring_buffer_destroy(ring_buffer **buffer) /*free in step*/
{
   if(*buffer == NULL) return;/*whether input is NULL*/
   free((*buffer)->content); /*free array first*/
   free(*buffer);            /*free the buffer after*/
   *buffer = NULL;
}

/* Returns true if the given ring buffer is empty, false otherwise */
bool ring_buffer_is_empty(const ring_buffer *buffer)
{
   if(buffer == NULL) return false;/*whether input is NULL*/
   return (buffer->read_pos == buffer->write_pos); /*judge the buffer empty*/
}

/* Read an element from the ring buffer (i.e., the dequeue operation), *data records
   the element popped. returns true if the operation succeeded, false otherwise. */
bool ring_buffer_read(ring_buffer *buffer, int *data)
{
   return ring_buffer_read_multi(buffer, 1, data); /*use ring_buffer_read_multi function*/
}

/* Write an element which value is `data` to the ring buffer (i.e., the enqueue operation),
   returns true if the operation succeeded, false otherwise. Note that when the buffer is
   full, grow its size to make sure the element can be write successfully. The detailed
   requirements of size growth are in the homework description. */
bool ring_buffer_write(ring_buffer *buffer, const int data)
{
   if(buffer == NULL) return false;/*whether input is NULL*/
   if (!(buffer->write_pos-buffer->read_pos==buffer->capacity)) /*just judge whether the buffer is full*/
   {
      buffer->content[((buffer->write_pos)++)%buffer->capacity] = data; /*write and update write_pos*/
      return true;
   }
   else /*the buffer is full*/
   {
      size_t cap = buffer->capacity;
      int *tmp;
      if (buffer->capacity < 1024) /*the two case of grow*/
         buffer->capacity *= RING_BUFFER_GROW_FACTOR1;
      else
         buffer->capacity *= RING_BUFFER_GROW_FACTOR2;
      tmp = (int *)calloc(buffer->capacity, sizeof(int));/*calloc memory to update content*/
      if (buffer->read_pos%cap < buffer->write_pos%cap)
         memcpy(tmp, buffer->content, cap * sizeof(int));
      else/*in this case we should cp the memory in two times to resort them*/
      {
         memcpy(tmp, buffer->content + buffer->read_pos%cap, (cap - (buffer->read_pos%cap)) * sizeof(int));
         memcpy(tmp + cap - buffer->read_pos%cap, buffer->content, (buffer->read_pos%cap) * sizeof(int));
      }
      buffer->read_pos = 0;/*reinit the read_pos*/
      buffer->write_pos = cap;/*reinit the write_pos*/
      tmp[((buffer->write_pos)++)%buffer->capacity] = data;/*write and update write_pos*/
      free(buffer->content);/*free old content*/
      buffer->content = tmp;/*update*/
      return true;
   }
   return false;
}

/* Read "rdsize" elements from the ring buffer, and fill them to "data".
   Notice that you should only return true when this operation succeeds.
   If there are not enough elements in the buffer, just DO NOTHING and
   return false. DO NOT modify the ring buffer and data for such cases. */
bool ring_buffer_read_multi(ring_buffer *buffer, size_t rdsize, int *data)
{
   size_t num = 0;
   if(buffer == NULL) return false;/*whether input is NULL*/
   if(data == NULL) return false;/*whether input is NULL*/
   num = buffer->write_pos - buffer->read_pos;/*count the num of number in buffer */
   if (num < rdsize) return false;/*not enough*/
   else
   {
      size_t i = 0;
      while (i < rdsize)
      {
         data[i++] = buffer->content[(buffer->read_pos++)%buffer->capacity];/*write to data and update index and read_pos(just like pop)*/
      }
   }
   return true;/*if we finish the part return true*/
}

/* Write "wrtsize" elements to the ring buffer. The values to write are
   provided in "data". Returns true if succeeds, false otherwise. Don't
   forget to grow your buffer size properly. */
bool ring_buffer_write_multi(ring_buffer *buffer, size_t wrtsize, const int *data)
{
   size_t i = 0;
   if(data == NULL) return false;/*whether input is NULL*/
      while (i < wrtsize)/*use while instead of for to avoid some warning*/
   {
      ring_buffer_write(buffer, data[i]);/*ring_buffer_write function to write one by one*/
      i++;
   }
   return true;
}

/* For every element in the ring buffer, apply "func" to it. For example,
   Your buffer currently contains 3 elements 1, 2, 4. If the map function is to
   "add five", then their values should be 6, 7, 9 after calling this function. */
bool ring_buffer_map(ring_buffer *buffer, map_func func)
{
   size_t index = buffer->read_pos%buffer->capacity;
   if(buffer == NULL) return false;/*whether input is NULL*/
   if (buffer->read_pos < buffer->write_pos)/*calculate it in two case*/
   {
      while (index < buffer->write_pos%buffer->capacity)
      {
         buffer->content[index] = func(buffer->content[index]);
         index++;/*to avoid ubs we cannot write the two lines in two lines*/
      }
      return true;/*if we finish the part return true*/
   }
   else /*calculate it in two case*/
   {
      while (index <= buffer->capacity && index > buffer->write_pos%buffer->capacity)
      {
         buffer->content[index] = func(buffer->content[index]);
         index++;/*to avoid ubs we cannot write the two lines in two lines*/
         if (index == buffer->capacity && index > buffer->write_pos%buffer->capacity)
         {
            index = 0;/*restart from the begin*/
            while (index < buffer->write_pos%buffer->capacity)
            {
               buffer->content[index] = func(buffer->content[index]);
               index++;/*to avoid ubs we cannot write the two lines in two lines*/
            }
         }
         break;/*to avoid some other problem*/
      }
      return true;/*if we finish the part return true*/
   }
   return false;/*if we cannot return before there must be some error*/
}
