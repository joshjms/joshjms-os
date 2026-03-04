#include <mm/pmm.h>

#include <stddef.h>

#include <limine/requests.h>
#include <memory.h>
#include <printk.h>

#define PAGE_SIZE 4096

static uint8_t *bitmap;
static size_t total_frames;
static size_t bitmap_size;
static size_t used_frames;

int pmm_init() {
    if(memmap_request.response == NULL) {
        printk("PMM: limine memmap request failed.\n");
        return -1;
    }

    if(hhdm_request.response == NULL) {
        printk("PMM: limine hhdm request failed.\n");
        return -1;
    }

    uint64_t hhdm_offset = hhdm_request.response->offset;

    struct limine_memmap_entry **entries = memmap_request.response->entries;
    size_t entry_count = memmap_request.response->entry_count;

    uint64_t highest_addr = 0;
    for(int i = 0; i < entry_count; i++) {
        uint64_t end_addr = entries[i]->base + entries[i]->length;
        if(end_addr > highest_addr) {
            highest_addr = end_addr;
        }
    }

    total_frames = highest_addr / PAGE_SIZE;
    bitmap_size = total_frames / 8 + 1;

    for(int i = 0; i < entry_count; i++) {
        if(entries[i]->type == LIMINE_MEMMAP_USABLE && entries[i]->length >= bitmap_size) {
            bitmap = (uint8_t *)(entries[i]->base + hhdm_offset);
            break;
        }
    }

    if(bitmap == NULL) {
        printk("PMM: no suitable memory region found for bitmap.\n");
        return -1;
    }

    for(size_t i = 0; i < bitmap_size; i++) {
        bitmap[i] = 0xFF;
    }

    for(int i = 0; i < entry_count; i++) {
        uint64_t base = entries[i]->base;
        uint64_t length = entries[i]->length;
        uint64_t type = entries[i]->type;

        if(type == LIMINE_MEMMAP_USABLE) {
            size_t start_frame = base / PAGE_SIZE;
            size_t end_frame = (base + length) / PAGE_SIZE;

            for(size_t frame = start_frame; frame < end_frame; frame++) {
                size_t byte_index = frame / 8;
                size_t bit_index = frame % 8;
                bitmap[byte_index] &= ~(1U << bit_index);
            }
        }
    }

    uint64_t bitmap_start = (uint64_t)bitmap - hhdm_offset;
    size_t bitmap_frames = (bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;
    for(size_t frame = bitmap_start / PAGE_SIZE; frame < (bitmap_start / PAGE_SIZE) + bitmap_frames; frame++) {
        size_t byte_index = frame / 8;
        size_t bit_index = frame % 8;
        bitmap[byte_index] |= (1U << bit_index);
    }

    return 0;
}

uint64_t pmm_alloc(void) {
    for(size_t i = 0; i < bitmap_size; i++) {
        if(bitmap[i] != 0xFF) {
            for(size_t bit = 0; bit < 8; bit++) {
                if((bitmap[i] & (1U << bit)) == 0) {
                    bitmap[i] |= (1U << bit);
                    used_frames++;
                    return (i * 8 + bit) * PAGE_SIZE;
                }
            }
        }
    }

    return UINT64_MAX;
}

void pmm_free(uint64_t addr) {
    size_t frame = addr / PAGE_SIZE;
    size_t byte_index = frame / 8;
    size_t bit_index = frame % 8;

    if((bitmap[byte_index] & (1U << bit_index)) != 0) {
        bitmap[byte_index] &= ~(1U << bit_index);
        used_frames--;
    }
}
