#pragma once

#include <memory>

class DataArea
{
    static constexpr size_t PAGE_LEN = 512;

public:
    bool initialize(uint32_t * const memory_pointer, const size_t size);

    const size_t get_size() const;

    void set_page(const uint32_t page_number, const uint32_t * const data, const size_t size);
    void get_page(const uint32_t page_number, uint32_t * const data, const size_t size);

public:
    static const size_t get_page_len() { return PAGE_LEN;}

private:
    uint32_t * _data;
    size_t	   _size;
};