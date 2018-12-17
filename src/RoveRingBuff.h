///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ROVE_RING_BUFF_H
#define ROVE_RING_BUFF_H

#include "stdint.h"
#include "stddef.h"

/////////////////////////////////
template<class TYPE, size_t SIZE> 

class RoveRingBuff
{
  public:

    TYPE     buffer_addr[SIZE];
    size_t   buffer_size = SIZE;
  
    uint32_t push_index = 0;
    uint32_t pop_index  = 0;
    size_t   cnt        = 0;

  public:

    size_t   count();
    bool     isEmpty();
    bool     isFull();
    
    TYPE peek();
    TYPE pop();
    void push(TYPE push);
};

//template<class TYPE, size_t SIZE> RoveRingBuff<TYPE, SIZE>::RoveRingBuff() {}

////////////////////////////////////////
template<class TYPE, size_t SIZE> 

size_t RoveRingBuff<TYPE, SIZE>::count() 
{ return this->cnt; }

////////////////////////////////////////
template<class TYPE, size_t SIZE> 

bool RoveRingBuff<TYPE, SIZE>::isEmpty()
{ return (cnt == 0); }

///////////////////////////////////////////////////////////////////////
template<class TYPE, size_t SIZE> 

bool RoveRingBuff<TYPE, SIZE>::isFull() 
{ return ((this->push_index == this->pop_index) && (this->cnt != 0)); }

/////////////////////////////////////////////////////////
template<class TYPE, size_t SIZE> 

TYPE RoveRingBuff<TYPE, SIZE>::peek()
{
  if(this->isEmpty())
  {
    return 0;

  } else {
    TYPE   pop_byte = this->buffer_addr[this->pop_index];
    return pop_byte;
  }
}


////////////////////////////////////////////////////////////////
template<class TYPE, size_t SIZE> 

TYPE RoveRingBuff<TYPE, SIZE>::pop()
{
  if(this->isEmpty()) {
    return 0;

  } else {
    TYPE  pop_byte  =  this->buffer_addr[this->pop_index];
    this->pop_index = (this->pop_index + 1) % this->buffer_size;
    this->cnt--;
    return pop_byte;
  }
}

///////////////////////////////////////////////////////////////////
template<class TYPE, size_t SIZE> 

void RoveRingBuff<TYPE, SIZE>::push(TYPE push)
{
  if(this->isFull())
  {
    return;

  } else {

    this->buffer_addr[this->push_index] = push;
    
    this->push_index = (this->push_index + 1) % this->buffer_size;
    this->cnt++;
    
    return;
  }
}

#endif // ROVE_RING_BUFF_H