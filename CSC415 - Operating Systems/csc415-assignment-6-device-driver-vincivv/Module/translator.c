/**************************************************************
* Class::  CSC-415-03 Spring 2024
* Name::  Dias Almat
* Student ID::  923022045
* GitHub-Name::  vincivv
* Project:: Assignment 6 – Coding A Device Driver
*
* File::  translator.c
*
* Description::  This is a Linux kernel module that implements a character device driver. The 
* device translates simple sentences between Russian and English based on a predefined dictionary. The 
* translation direction can be set via an ioctl command. Users can write sentences to the device and 
* read back the translated sentences. File also contains basic functionality for device registration and
* cleanup.
*
**************************************************************/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/string.h>

#define BUF_LEN 256
#define DEVICE_NAME "ruen_translator"
#define IOCTL_SET_MODE _IOW('t', 'm', int32_t*)

//Metadata
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("RU/EN translator device driver");
MODULE_AUTHOR("Dias Almat");

static int majorNumber;
static char out_buffer[BUF_LEN];
static int current_mode = 0; // 0 for RU to EN, 1 for EN to RU
static struct class *cls;

struct dictionary_entry {
    const char *ru;
    const char *en;
};


//Dictionary words
static struct dictionary_entry dictionary[] = {
    {"привет", "hello"},
    {"мир", "world"},
    {"как", "how"},
    {"", "are"},
    {"ты", "you"},
    {"тебя", "you"},
    {"я", "I"},
    {"дела", "things"},
    {"спасибо", "thanks"},
    {"пожалуйста", "please"},
    {"досвидания", "goodbye"},
    {"да", "yes"},
    {"нет", "no"},
    {"извините", "sorry"},
    {"пока", "bye"},
    {"доброе", "good"},
    {"утро", "morning"},
    {"день", "afternoon"},
    {"вечер", "evening"},
    {"ночи", "night"},
    {"хорошо", "well"},
    {"плохо", "bad"},
    {"любовь", "love"},
    {"люблю", "love"},
    {"дружба", "friendship"},
    {"друг", "friend"},
    {"семья", "family"}, 
    {"дом", "home"},
    {"работа", "work"},
    {"школа", "school"},
    {"университет", "university"},
    {"город", "city"},
    {"страна", "country"},
    {"еда", "food"},
    {"вода", "water"},
    {"чай", "tea"},
    {"кофе", "coffee"},
    {"счастье", "happiness"},
    {"печаль", "sadness"},
    {"здоровье", "health"},
    {"деньги", "money"},
};

static int dict_size = sizeof(dictionary) / sizeof(dictionary[0]) - 1;

//translate a single word
static void translate_word(const char *word, char *dest, int dest_size) {
    for (int i = 0; i < dict_size; i++) {
        if (current_mode == 0 && strcmp(word, dictionary[i].ru) == 0) {
            strncpy(dest, dictionary[i].en, dest_size);
            dest[dest_size - 1] = '\0'; // null termination
            return;
        } else if (current_mode == 1 && strcmp(word, dictionary[i].en) == 0) {
            strncpy(dest, dictionary[i].ru, dest_size);
            dest[dest_size - 1] = '\0'; 
            return;
        }
    }
    strncpy(dest, word, dest_size-1); // If not found, return the original word
    dest[dest_size - 1] = '\0'; 
}

//translate a sentence
static void translate_sentence(const char *input) {
    char word[32];
    int input_i = 0;
    int output_i = 0;
    int word_i = 0;
    char ch;

    memset(out_buffer, 0, BUF_LEN);

    while(input_i < BUF_LEN -1 && input[input_i] != '\0') {
        ch = input[input_i];
        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\0') {
            if (word_i > 0) {
                word[word_i] = '\0';
                char translated[32];
                translate_word(word, translated, sizeof(translated));
                if (output_i + strlen(translated) < BUF_LEN - 1) {
                    strcpy(&out_buffer[output_i], translated);
                    output_i += strlen(translated);
                }
                word_i = 0;
            }
            if (output_i < BUF_LEN - 1) {
                out_buffer[output_i++] = ch;
            }
        } else {
            if (word_i < sizeof(word) - 1) {
                word[word_i++] = ch;
            }
        }
        input_i++;
    }

    //handle last word if no trailing space
    if (word_i > 0) {
        word[word_i] = '\0';
        char translated[32];
        translate_word(word, translated, sizeof(translated));
        if (output_i + strlen(translated) < BUF_LEN - 1) {
            strcpy(&out_buffer[output_i], translated);
            output_i += strlen(translated);
        }
    }
    out_buffer[output_i] = '\0';

}

//File operations
static int dev_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Translator: device opened\n");
    return 0;
}
static int dev_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Translator: device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    size_t bytes_to_copy;
    if (*offset != 0) {
        return 0; // EOF
    }

    if(len > BUF_LEN) {
        len = BUF_LEN;
    }

    bytes_to_copy = copy_to_user(buf, out_buffer, len);
    if(bytes_to_copy != 0){
        printk(KERN_ERR "translator: failed to send data to user\n");
        return -EFAULT;
    }

    printk(KERN_INFO "translator: sent to user: %s\n", out_buffer);
    *offset += len;
    return len;
}

//write a sentence to translate
static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *offset){
    char in_buffer[BUF_LEN];

    if(len > BUF_LEN - 1){
        len = BUF_LEN - 1;
    }

    if(copy_from_user(in_buffer, buf, len) != 0){
        printk(KERN_ERR "translator: failed to receive data from user\n");
        return -EFAULT;
    }

    in_buffer[len] = '\0';
    printk(KERN_INFO "translator: received from user: %s\n", in_buffer);
    translate_sentence(in_buffer);
    printk(KERN_INFO "translator: translated (%s): %s\n", current_mode == 0 ? "RU to EN" : "EN to RU", out_buffer);
    return len;
}

static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    int32_t mode;
    if(cmd == IOCTL_SET_MODE){
        if(copy_from_user(&mode, (int32_t __user *)arg, sizeof(mode)) != 0){
            printk(KERN_ERR "translator: failed to set mode from user\n");
            return -EFAULT;
        }

        if(mode == 0 || mode == 1){
            current_mode = mode;
            printk(KERN_INFO "translator: mode set to %s\n", current_mode == 0 ? "RU to EN" : "EN to RU");
        } else {
            printk(KERN_ERR "translator: invalid mode %d\n", mode);
            return -EINVAL;
        }
    }
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
    .unlocked_ioctl = dev_ioctl,
};

//module initialization / exit
static int __init translator_init(void){
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if(majorNumber < 0){
        printk(KERN_ALERT "translator failed to register a major number\n");
        return majorNumber;
    }

    cls = class_create(DEVICE_NAME);
    if (IS_ERR(cls)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ERR "translator: failed to create class\n");
        return PTR_ERR(cls);
    }

    device_create(cls, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    printk(KERN_INFO "translator: loaded with major number %d\n", majorNumber);

    //default mode
    current_mode = 0;

    return 0;
}

static void __exit translator_exit(void){
    device_destroy(cls, MKDEV(majorNumber, 0));
    class_destroy(cls);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "translator: unloaded\n");
}

module_init(translator_init);
module_exit(translator_exit);