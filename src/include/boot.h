#pragma once

#include <stdint.h>
#include <vbe.h>

namespace nov
{
namespace boot
{

#pragma pack (1)
struct nov_mmap_table_entry
{
    uint64_t region_base;
    uint64_t region_size;
    uint32_t region_type;
    uint32_t region_acpi;
};
#pragma pack (0)

#pragma pack (1)
struct nov_os_hint_table
{
    void* gdt_address;
    uint32_t low_kilobyte_blocks;
    uint32_t cpuid_ecx_content;
    uint32_t cpuid_edx_content;
    nov_mmap_table_entry* memory_map_table_address;
    uint16_t memory_map_table_entries;
    uint16_t boot_disk_number;
    void* bios_data_area_address;
    void* kernel_elf_start;
    void* kernel_elf_end;
    vbe::nov_vbe_mode_info* vbe_mode_info_block;
    uint32_t checksum;
    uint32_t checksum_novo;
};
#pragma pack (0)

}
}