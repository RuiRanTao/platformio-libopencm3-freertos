
#define GPIO_USART1_TX GPIO9
#define GPIO_USART1_RX GPIO10

typedef int32_t ring_size_t;

struct ring {
	uint8_t *data;
	ring_size_t size;
	uint32_t begin;
	uint32_t end;
};

#define RING_SIZE(RING)  ((RING)->size - 1)
#define RING_DATA(RING)  (RING)->data
#define RING_EMPTY(RING) ((RING)->begin == (RING)->end)

#define BUFFER_SIZE 1024

extern unsigned char rc ;
void usart_setup(void);
