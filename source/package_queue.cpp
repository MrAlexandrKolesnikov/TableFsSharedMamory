#include "package_queue.h"

#include <chrono>
#include <iostream>


bool PackegeQueue::initialize(const std::string& name, const size_t size)
{
    const std::string memory_name = name + "_memory";
    const std::string queue_name = name + "_queue";
    boost::interprocess::permissions permission;
    permission.set_unrestricted();

    _memory = std::make_unique<SharedMemory>(boost::interprocess::open_or_create
                                                  , memory_name.c_str()
                                                  , boost::interprocess::read_write
                                                  , size);
    _memory->truncate(size);
    
    _region = std::make_unique<MappedRegion>(*_memory.get(), boost::interprocess::read_write);
    uint32_t * free_space = static_cast<uint32_t*>(_region.get()->get_address());


    memset(free_space, 0, size);

    if (!_page_manager.initialize(free_space, size))
        return false;

    const uint32_t number_of_page = _page_manager.get_number_of_page();

    _queue = std::make_unique<MessageQueue>(boost::interprocess::open_or_create
                               , queue_name.c_str()
                               , number_of_page
                               , sizeof(FileDescription));
    
    

    return true;
}

bool PackegeQueue::push(uint32_t * const data, const size_t size)
{
    _file_index++;
    _file_index %= _queue->get_max_msg();
    if (_file_index == 0) 
        _file_index++;

    uint32_t table_offset = 0;
    const bool success = _page_manager.write(data, size, _file_index, table_offset);
    if (success)
    {
        FileDescription file = { _file_index, table_offset };
        
        return _queue->try_send(&file, sizeof(file), 0);
    }
    return false;
}

bool PackegeQueue::pop(uint32_t * const data, size_t size)
{
    FileDescription file;
    unsigned int priority = 0;
    size_t description_size = sizeof(FileDescription);
    const bool is_receive = _queue->try_receive(&file, description_size, description_size, priority);

    if (!is_receive)
        return false;

    _page_manager.read(data,file.tabel_offset);
    return true;
}
