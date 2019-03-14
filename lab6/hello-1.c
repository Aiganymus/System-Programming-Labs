#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <asm/io.h>

const uint8_t lower_ascii_codes[256] = {
    0x00,  'E',  '1',  '2',     /* 0x00 */
     '3',  '4',  '5',  '6',     /* 0x04 */
     '7',  '8',  '9',  '0',     /* 0x08 */
     '-',  '=',   'B', '\t',     /* 0x0C */
     'q',  'w',  'e',  'r',     /* 0x10 */
     't',  'y',  'u',  'i',     /* 0x14 */
     'o',  'p',  '[',  ']',     /* 0x18 */
    '\n', 0x00,  'a',  's',     /* 0x1C */
     'd',  'f',  'g',  'h',     /* 0x20 */
     'j',  'k',  'l',  ';',     /* 0x24 */
    '\'',  '`', 0x00, '\\',     /* 0x28 */
     'z',  'x',  'c',  'v',     /* 0x2C */
     '.',  '/', 0x00,  '*',     /* 0x34 */
    0x00,  ' ', 0x00, 0x00,     /* 0x38 */
    0x00, 0x00, 0x00, 0x00,     /* 0x3C */
    0x00, 0x00, 0x00, 0x00,     /* 0x40 */
    0x00, 0x00, 0x00,  '7',     /* 0x44 */
     '8',  '9',  '-',  '4',     /* 0x48 */
     '5',  '6',  '+',  '1',     /* 0x4C */
     '2',  '3',  '0',  '.',     /* 0x50 */
    0x00, 0x00, 0x00, 0x00,     /* 0x54 */
    0x00, 0x00, 0x00, 0x00      /* 0x58 */
};

// Scancode -> ASCII
const uint8_t upper_ascii_codes[256] = {
    0x00,  'E',  '!',  '@',     /* 0x00 */
     '#',  '$',  '%',  '^',     /* 0x04 */
     '&',  '*',  '(',  ')',     /* 0x08 */
     '_',  '+',   'B', '\t',     /* 0x0C */
     'Q',  'W',  'E',  'R',     /* 0x10 */
     'T',  'Y',  'U',  'I',     /* 0x14 */
     'O',  'P',  '{',  '}',     /* 0x18 */
    '\n', 0x00,  'A',  'S',     /* 0x1C */
     'D',  'F',  'G',  'H',     /* 0x20 */
     'J',  'K',  'L',  ':',     /* 0x24 */
     '"',  '~', 0x00,  '|',     /* 0x28 */
     'Z',  'X',  'C',  'V',     /* 0x2C */
     'B',  'N',  'M',  '<',     /* 0x30 */
     '>',  '?', 0x00,  '*',     /* 0x34 */
    0x00,  ' ', 0x00, 0x00,     /* 0x38 */
    0x00, 0x00, 0x00, 0x00,     /* 0x3C */
    0x00, 0x00, 0x00, 0x00,     /* 0x40 */
    0x00, 0x00, 0x00,  '7',     /* 0x44 */
     '8',  '9',  '-',  '4',     /* 0x48 */
     '5',  '6',  '+',  '1',     /* 0x4C */
     '2',  '3',  '0',  '.',     /* 0x50 */
    0x00, 0x00, 0x00, 0x00,     /* 0x54 */
    0x00, 0x00, 0x00, 0x00      /* 0x58 */
};


const uint8_t lower_ascii_codes_dvorak[256] = {
    0x00,  'E',  '1',  '2',     /* 0x00 */
     '3',  '4',  '5',  '6',     /* 0x04 */
     '7',  '8',  '9',  '0',     /* 0x08 */
     '[',  ']',   'B', '\t',     /* 0x0C */
    '\'',  ',',  '.',  'p',     /* 0x10 */
     'y',  'f',  'g',  'c',     /* 0x14 */
     'r',  'l',  '/',  '=',     /* 0x18 */
    '\n', 0x00,  'a',  'o',     /* 0x1C */
     'e',  'u',  'i',  'd',     /* 0x20 */
     'h',  't',  'n',  's',     /* 0x24 */
     '-',  '`', 0x00, '\\',     /* 0x28 */
     ';',  'q',  'j',  'k',     /* 0x2C */
     'x',  'b',  'm',  'w',     /* 0x30 */
     'v',  'z', 0x00,  '*',     /* 0x34 */
    0x00,  ' ', 0x00, 0x00,     /* 0x38 */
    0x00, 0x00, 0x00, 0x00,     /* 0x3C */
    0x00, 0x00, 0x00, 0x00,     /* 0x40 */
    0x00, 0x00, 0x00,  '7',     /* 0x44 */
     '8',  '9',  '[',  '4',     /* 0x48 */
     '5',  '6',  '}',  '1',     /* 0x4C */
     '2',  '3',  '0',  'v',     /* 0x50 */
    0x00, 0x00, 0x00, 0x00,     /* 0x54 */
    0x00, 0x00, 0x00, 0x00      /* 0x58 */
};

// Scancode -> ASCII
const uint8_t upper_ascii_codes_dvorak[256] = {
    0x00,  'E',  '!',  '@',     /* 0x00 */
     '#',  '$',  '%',  '^',     /* 0x04 */
     '&',  '*',  '(',  ')',     /* 0x08 */
     '{',  '}',   'B', '\t',     /* 0x0C */
     '"',  '<',  '>',  'P',     /* 0x10 */
     'Y',  'F',  'G',  'C',     /* 0x14 */
     'R',  'L',  '?',  '+',     /* 0x18 */
    '\n', 0x00,  'A',  'O',     /* 0x1C */
     'E',  'U',  'I',  'D',     /* 0x20 */
     'H',  'T',  'N',  'S',     /* 0x24 */
     '_',  '~', 0x00,  '|',     /* 0x28 */
     ':',  'Q',  'J',  'K',     /* 0x2C */
     'X',  'B',  'M',  'W',     /* 0x30 */
     'V',  'Z', 0x00,  '*',     /* 0x34 */
    0x00,  ' ', 0x00, 0x00,     /* 0x38 */
    0x00, 0x00, 0x00, 0x00,     /* 0x3C */
    0x00, 0x00, 0x00, 0x00,     /* 0x40 */
    0x00, 0x00, 0x00,  '7',     /* 0x44 */
     '8',  '9',  '[',  '4',     /* 0x48 */
    '5',  '6',  '}',  '1',     /* 0x4C */
     '2',  '3',  '0',  'v',     /* 0x50 */
    0x00, 0x00, 0x00, 0x00,     /* 0x54 */
    0x00, 0x00, 0x00, 0x00      /* 0x58 */
};

// shift flags. left shift is bit 0, right shift is bit 1.
uint8_t shift;
// control flags just like shift flags.
uint8_t ctrl;
uint8_t keypresses[256];

#define BUFFLEN 128
// New characters are added to hd. characters are pulled off of tl.
uint8_t kb_buff[BUFFLEN];
uint8_t kb_buff_hd;
uint8_t kb_buff_tl;

int irqn = 1;

irqreturn_t my_interrupt(int irq, void *dev, struct pt_regs *regs) {
static unsigned char byte;
  unsigned char status;

  status = inb(0x64);
  byte = inb(0x60);
   /* printk(KERN_INFO "you pressed %x\n", scancode);
  switch (scancode) {
      case 0x01:  
        printk (KERN_INFO "! You pressed Esc ...\n");
        break;
      case 0x3B:  
        printk (KERN_INFO "! You pressed F1 ...\n");
        break;
      case 0x3C:  
        printk (KERN_INFO "! You pressed F2 ...\n");
        break;
      default:
        break;
  }*/

//    uint8_t byte = inb(0x60);

    /*if(byte & 0x80) {
        // Key release
        uint8_t pressedbyte = byte & 0x7F;
        // Check if we're releasing a shift key.
        if(pressedbyte == 0x2A) {
            // left
            shift = shift & 0x02;
        }
        else if(pressedbyte == 0x36) {
            // right
            shift = shift & 0x01;
        }
        else if(pressedbyte == 0x1D) {
            ctrl = 0;
        }

        keypresses[pressedbyte] = 0;
    }

    if(keypresses[byte] < 10 && keypresses[byte] > 0) {
        // Key is already pressed. Ignore it.
        keypresses[byte]++; // Increment anyway, so we can roll over and repeat.

    }
    keypresses[byte]++;

    if(byte == 0x2A) {
        shift = shift | 0x01;
    }
    else if(byte == 0x36) {
        shift = shift | 0x02;
    }
    else if(byte == 0x1D) {
        ctrl = 1;
    }

    const uint8_t *codes;
    if(shift) {
        codes = upper_ascii_codes_dvorak;
    }
    else {
        codes = lower_ascii_codes_dvorak;
    }

    uint8_t ascii = codes[byte];*/
    uint8_t ascii = lower_ascii_codes_dvorak[byte];
    printk(KERN_INFO "you pressed %c", ascii);

    return IRQ_HANDLED;
}

int init_module(void) {
    free_irq(irqn, NULL);

    if (request_irq(1, (irq_handler_t) my_interrupt, IRQF_SHARED, "print_key", (void *)(my_interrupt))) {
        printk(KERN_ERR "print_key: cannot register IRQ %d\n", irqn);
        return -EIO;
    }
	return 0;
}

void cleanup_module(void)
{
    free_irq(irqn, (void *)(my_interrupt));
	printk(KERN_INFO "Hello: goodbye.\n");
}

