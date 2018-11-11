#include "allocation_table.h"
#include "../data_area/data_area.h"

uint32_t * AllocationTable::initialize(uint32_t * memory_pointer, size_t size)
{
    if (memory_pointer == nullptr)
        return 0;

    const size_t page_len = DataArea::get_page_len();
    uint32_t * _data_pointer = nullptr;

    _table = memory_pointer;
    _number_of_page = size / (page_len * sizeof(uint32_t));
    _data_pointer  = _table + (_number_of_page * sizeof(DescriptionType));

    _number_of_page -= (_data_pointer - _table) / page_len;

    _tabel_size = _number_of_page * sizeof(DescriptionType);

    for(size_t table_offset = 0; table_offset < _number_of_page; table_offset++)
    {
        *get_table(table_offset,PAGE_NUMBER_OFFSET) = table_offset;
        clear_descriptor(table_offset);
    }

    return _data_pointer;
}

void AllocationTable::write_descriptor(const size_t table_offset, const size_t data_size, const uint32_t file_index)
{
    *get_table(table_offset, FILE_INDEX_OFFSET) = file_index;
    *get_table(table_offset, SIZE_OFFSET) = data_size;
}

void AllocationTable::clear_descriptor(const size_t table_offset)
{
    *get_table(table_offset, FILE_INDEX_OFFSET) = 0;
    *get_table(table_offset, SIZE_OFFSET) = 0;
    *get_table(table_offset, NEXT_CELL_OFFSET) = END_OF_FILE;
}

uint32_t AllocationTable::get_first_empty_cell(const size_t table_offset)
{
    size_t table_cell = table_offset;
    while (!is_empty(table_cell) && table_cell < _number_of_page)
        table_cell++;
    return table_cell;
}

uint32_t AllocationTable::get_page_number(const size_t table_offset) const
{
    return *get_table(table_offset, PAGE_NUMBER_OFFSET);
}

uint32_t AllocationTable::get_file_index(const size_t table_offset) const
{
    return *get_table(table_offset, FILE_INDEX_OFFSET);
}

uint32_t AllocationTable::get_data_size(const size_t table_offset) const
{
    return *get_table(table_offset, SIZE_OFFSET);
}

uint32_t AllocationTable::get_next_cell(const size_t table_offset) const
{
    return *get_table(table_offset, NEXT_CELL_OFFSET);
}

void AllocationTable::set_next_cell(const size_t table_offset, const size_t next_page)
{
    *get_table(table_offset, NEXT_CELL_OFFSET) = next_page;
}

void AllocationTable::set_as_last_cell(const size_t table_offset)
{
    *get_table(table_offset, NEXT_CELL_OFFSET) = END_OF_FILE;
}

bool AllocationTable::is_empty(const size_t table_offset) const
{
    return *get_table(table_offset, FILE_INDEX_OFFSET) == 0;
}

bool AllocationTable::is_end_of_file(const size_t table_offset) const
{
    return get_next_cell(table_offset) == END_OF_FILE;
}
