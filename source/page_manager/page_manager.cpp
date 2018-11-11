#include "page_manager.h"

bool PageManager::initialize(uint32_t * const memory_pointer, const size_t size)
{
    _table = std::make_unique<AllocationTable>();
    _data = std::make_unique<DataArea>();

    uint32_t * _data_pointer = _table->initialize(memory_pointer, size);
    size_t data_size = size - _table->get_tabel_size();

    return _data->initialize(_data_pointer, data_size);
}

bool PageManager::write(uint32_t * const data, const size_t size, uint32_t const file_index, uint32_t &first_cell)
{
    if (_data->get_size() < size)
        return false;
    
    size_t	   data_size = size;
    uint32_t * data_pointer = data;

    uint32_t   table_cell = 0;

    uint32_t   previous_table_cell = table_cell;

    size_t const page_len = DataArea::get_page_len();

    bool is_first_cell = true;

    while (data_size > page_len)
    {
        table_cell = _table->get_first_empty_cell(table_cell);
        _table->write_descriptor(table_cell, page_len, file_index);
        if (is_first_cell)
        {
            is_first_cell = false;
            first_cell = table_cell;
        }
        else
            _table->set_next_cell(previous_table_cell, table_cell);

        _data->set_page(_table->get_page_number(table_cell), data_pointer, page_len);
        previous_table_cell = table_cell;
    }

    if (data_size != 0)
    {
        table_cell = _table->get_first_empty_cell(table_cell);
        _table->write_descriptor(table_cell, page_len, file_index);

        if (is_first_cell)
        {
            is_first_cell = false;
            first_cell = table_cell;
        }
        else
            _table->set_next_cell(previous_table_cell, table_cell);

        _data->set_page(_table->get_page_number(table_cell), data_pointer, page_len);
        previous_table_cell = table_cell;
    }

    _table->set_as_last_cell(table_cell);

    return true;
}

void PageManager::read(uint32_t * const data, uint32_t table_offset)
{
    uint32_t   table_cell = table_offset;
    uint32_t   previous_table_cell = table_cell;

    uint32_t *  data_pointer = data;

    do
    {
        _data->get_page(_table->get_page_number(table_cell),data_pointer, _table->get_data_size(table_cell));
        data_pointer += _table->get_data_size(table_cell);

        if (!_table->is_end_of_file(table_cell))
        {
            previous_table_cell = table_cell;
            table_cell = _table->get_next_cell(table_cell);
            _table->clear_descriptor(previous_table_cell);
        }
        else
        {   
            _table->clear_descriptor(table_cell);
            break;
        }

    } while (true);
}

uint32_t PageManager::get_number_of_page() const
{
    return _table->get_number_of_page();
}




