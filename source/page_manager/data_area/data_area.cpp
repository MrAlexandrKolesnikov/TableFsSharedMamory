#include "data_area.h"

bool DataArea::initialize(uint32_t * const memory_pointer, const size_t size)
{
    if (size < PAGE_LEN)
        return false;

    _data = memory_pointer;
    _size = size;
    return true;
}

const size_t DataArea::get_size() const
{
    return _size;
}

void DataArea::set_page(const uint32_t page_number, const uint32_t * const data, const size_t size)
{
    _size += PAGE_LEN;
    memcpy((_data + (PAGE_LEN * page_number)), data, size * sizeof(uint32_t));
}

void DataArea::get_page(const uint32_t page_number, uint32_t * const data, const size_t size)
{
    _size -= PAGE_LEN;
    memcpy(data, _data + (PAGE_LEN * page_number), size * sizeof(uint32_t));
}
