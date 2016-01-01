template < class IndexType, class EntryType, unsigned Size >
class CircularBuffer
{
public:

    CircularBuffer()
    {
        m_getIdx = m_putIdx = 0;
    }

    IndexType Len() const   { return m_putIdx - m_getIdx; }

    bool IsEmpty() const    { return Len() == 0; }
    bool IsFull() const     { return Len() == Size; }
    bool Error() const      { return Len() > Size; }

    bool Push( EntryType val )
    {
        if (IsFull())
        {
        	return false;
        }
        else
        {
        	m_entry[ m_putIdx++ % ( Size - 1 )] = val;
        	return true;
        }
    }
    bool Peek(EntryType& peekValue)
    {
        if (IsEmpty())
        {
        	return false;
        }
        else
        {
        	peekValue =  m_entry[ m_getIdx % ( Size - 1 )];
        	return true;
        }

    }

    // TODO make atomic
    bool Pop(EntryType& popValue)
    {
        if (IsEmpty())
        {
        	m_getIdx 	= 0;
        	m_putIdx 	= 0;
        	return false;
        }
        else
        {
        	popValue =  m_entry[ m_getIdx++ % ( Size - 1 )];
        	return true;
        }
    }

private:

    volatile IndexType  m_getIdx;
    volatile IndexType  m_putIdx;
    EntryType           m_entry[ Size ];

};
