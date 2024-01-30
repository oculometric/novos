#include <window.h>

#include <serial.h>

static inline uint16_t get_slice_offset(const nov_uvector2& local_position, const nov_uvector2& size)
{
    uint8_t bitpacked = // in order from lowest bit to highest bit
        ((local_position.u < WINDOW_SLICE_SIZE)) |
        ((local_position.v < WINDOW_SLICE_SIZE) << 1) |
        ((size.u-local_position.u < WINDOW_SLICE_SIZE) << 2) |
        ((size.v-local_position.v < WINDOW_SLICE_SIZE) << 3);
    
    switch (bitpacked)
    {
    case 0b0000: return WINDOW_SOLID*WINDOW_SLICE_AREA;
    case 0b0001: return WINDOW_LEFTEDGE*WINDOW_SLICE_AREA;
    case 0b0010: return WINDOW_TOPEDGE*WINDOW_SLICE_AREA;
    case 0b0011: return WINDOW_TOPLEFTCORNER*WINDOW_SLICE_AREA;
    case 0b0100: return WINDOW_RIGHTEDGE*WINDOW_SLICE_AREA;
    case 0b0110: return WINDOW_TOPRIGHTCORNER*WINDOW_SLICE_AREA;
    case 0b1000: return WINDOW_BOTTOMEDGE*WINDOW_SLICE_AREA;
    case 0b1100: return WINDOW_BOTTOMRIGHTCORNER*WINDOW_SLICE_AREA;
    case 0b1001: return WINDOW_BOTTOMLEFTCORNER*WINDOW_SLICE_AREA;
    default: return WINDOW_SOLID*WINDOW_SLICE_AREA;
    }
}

void draw_window(const nov_uvector2& origin, const nov_uvector2& size, uint8_t* buffer, const nov_uvector2& buffer_size)
{
    nov_uvector2 local_position;
    nov_uvector2 global_position = origin;

    uint32_t global_index = (origin.v*buffer_size.u)+origin.u;
    uint16_t mod_index = 0;
    uint16_t slice_offset = 0;
    uint8_t value = 0;
    nov_uvector3 colour;
    while (local_position.u < size.u && local_position.v < size.v)
    {
        slice_offset = get_slice_offset(local_position, size);
        value = sliced_window[slice_offset+((local_position.v%5)*5)+(local_position.u%5)];
        colour = colour_palette[value];
        buffer[(global_index*3)+0] = colour.z;
        buffer[(global_index*3)+1] = colour.y;
        buffer[(global_index*3)+2] = colour.x;

        local_position.u++;
        global_position.u++;
        global_index++;
        if (local_position.u >= size.u)
        {
            local_position.u = 0;
            local_position.v++;

            global_position.u = origin.u;
            global_position.v++;

            global_index -= size.u;
            global_index += buffer_size.u;
        }
        mod_index++;
        mod_index = mod_index%5;
        //global_index = (global_position.v*buffer_size.u)+global_position.u;
        //serial_print((char*)"global pos: ", COM1); serial_print_dec(global_position.u, COM1); serial_print((char*)", ", COM1); serial_println_dec(global_position.v, COM1);
    }
}