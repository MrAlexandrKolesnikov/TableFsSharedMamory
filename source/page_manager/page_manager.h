#pragma once

#include "allocation_table/allocation_table.h"
#include "data_area/data_area.h"

class PageManager {
    using DataAreaUnique = std::unique_ptr<DataArea>;
    using AllocationTableUnique = std::unique_ptr<AllocationTable>;

public:
    bool initialize(uint32_t * const memory_pointer, const size_t size);

    bool write(uint32_t * const data, const size_t size, uint32_t const file_index, uint32_t &first_cell);
    void read(uint32_t * const data, uint32_t table_offset);

    uint32_t get_number_of_page() const;

private:
    DataAreaUnique         _data;
    AllocationTableUnique  _table;
};
