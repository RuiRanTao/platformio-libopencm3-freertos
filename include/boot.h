typedef void(*pFunction)(void);
#define ApplicationAddress    0x08000000
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */
#define __STATIC_INLINE                   static inline


#define SEND_BUFFER_SIZE 2048
#define FLASH_OPERATION_ADDRESS ((uint32_t)0x08006000)
#define FLASH_PAGE_NUM_MAX 127
#define FLASH_PAGE_SIZE 0x800
#define FLASH_WRONG_DATA_WRITTEN 0x80
#define RESULT_OK 0

extern uint8_t  entery_flag ;
extern uint8_t  str_program[3];
extern uint8_t  str_gotoapp[3];

void boot_entery(void) ;
uint32_t flash_program_data(uint32_t start_address, uint8_t *input_data, uint16_t num_elements);
void flash_read_data(uint32_t start_address, uint16_t num_elements, uint8_t *output_data);
void usart_send_string(uint32_t usart, uint8_t *string, uint16_t str_size);
void usart_get_string(uint32_t usart, uint8_t *out_string, uint16_t str_max_size);
void local_ltoa_hex(uint32_t value, uint8_t *out_string);
