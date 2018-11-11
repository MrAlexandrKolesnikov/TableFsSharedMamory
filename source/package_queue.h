#pragma once

#include <memory>
#include <cstring>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/smart_ptr/make_unique.hpp>

#include "page_manager/page_manager.h"

class PackegeQueue
{
    using MessageQueue = boost::interprocess::message_queue; 
    using SharedMemory = boost::interprocess::shared_memory_object;
    using MappedRegion = boost::interprocess::mapped_region;

    using MessageQueueUnique = std::unique_ptr<MessageQueue>;
    using SharedMemoryUnique = std::unique_ptr<SharedMemory>;
    using MappedRegionUnique = std::unique_ptr<MappedRegion>;

    struct FileDescription
    {
        uint32_t index;
        uint32_t tabel_offset;
    };

public:
    explicit PackegeQueue() = default;
    virtual ~PackegeQueue() = default;

    bool initialize(const std::string& name,const size_t size);

    bool pop(uint32_t * const data, const size_t size);
    bool push(uint32_t * const data, size_t  size);

private:
    uint32_t _file_index = { 1 };
    MessageQueueUnique  _queue;
    SharedMemoryUnique  _memory;
    MappedRegionUnique  _region;
    PageManager         _page_manager;
};
