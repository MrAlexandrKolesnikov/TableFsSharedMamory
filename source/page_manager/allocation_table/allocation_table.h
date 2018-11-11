#pragma once

#include <memory>

class AllocationTable
{
    using DescriptionType = size_t;

    static constexpr size_t FILE_INDEX_OFFSET = 0;
    static constexpr size_t PAGE_NUMBER_OFFSET = 1;
    static constexpr size_t SIZE_OFFSET = 2;
    static constexpr size_t NEXT_CELL_OFFSET = 3;
	
    static constexpr size_t END_OF_FILE = UINT_LEAST32_MAX;

public:
    explicit AllocationTable() = default;
    virtual ~AllocationTable() = default;

    uint32_t * initialize(uint32_t * memory_pointer, const size_t size);

    uint32_t get_number_of_page() const { return _number_of_page; };

    uint32_t get_tabel_size() const { return _tabel_size; };

    void write_descriptor(const size_t table_offset, const size_t data_size, const uint32_t file_index);
    void clear_descriptor(const size_t table_offset);

    uint32_t get_first_empty_cell(const size_t table_offset);

    uint32_t get_page_number(const size_t table_offset) const;
    uint32_t get_file_index(const size_t table_offset) const;
    uint32_t get_data_size(const size_t table_offset) const;
    uint32_t get_next_cell(const size_t table_offset) const;

    void set_next_cell(const size_t table_offset, const size_t next_page);
    void set_as_last_cell(const size_t table_offset);

    bool is_empty(const size_t table_offset) const;
    bool is_end_of_file(const size_t table_offset) const;

private:

    inline uint32_t* get_table(const size_t table_offset, const size_t global_offset)
    {
        return const_cast<uint32_t*>(static_cast<const AllocationTable*>(this)->get_table(table_offset, global_offset));
    }

    inline const uint32_t* const get_table(const size_t table_offset, const size_t global_offset) const
    {
        const size_t TABLE_OFFSET = table_offset* sizeof(DescriptionType);
		return _table + TABLE_OFFSET + global_offset;
	}

private:
    uint32_t * _table = nullptr;
    size_t _tabel_size;
    size_t _number_of_page;
};