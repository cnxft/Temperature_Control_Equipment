#include "AT24Cxx/at24cxx.h"
#include <string.h>

/**
 * @brief        At24Cxx任意地址写一个字节数据
 * @param        addr —— 写数据的地址（0-255）--本次使用AT24C64，写数据的地址（0-8189），不同芯片大小在EEPROM_IIC_AT24CXX.h中有定义
 * @param        dat  —— 存放写入数据的地址
 * @retval       成功 —— HAL_OK
 */
HAL_StatusTypeDef At24Cxx_Write_Byte(uint16_t addr, uint8_t *dat)
{
    HAL_StatusTypeDef result;
    result = HAL_I2C_Mem_Write(AT24Cxx_I2C, AT24CXX_ADDR_WRITE, addr, I2C_MEMADD_SIZE, dat, 1, 0xFFFF);
#if SYSTEM_TIME == HAL_BARE_MACHINE
    HAL_Delay(5); // 写一个字节，延迟一段时间，不能连续写
#elif SYSTEM_TIME == HAL_FREERTOS
    osDelay(5);
#endif
    return result;
}

/**
 * @brief        At24Cxx任意地址读一个字节数据
 * @param        addr —— 读数据的地址（0-255）
 * @param        read_buf —— 存放读取数据的地址
 * @retval       成功 —— HAL_OK
 */
HAL_StatusTypeDef At24Cxx_Read_Byte(uint16_t addr, uint8_t *read_buf)
{
    return HAL_I2C_Mem_Read(AT24Cxx_I2C, AT24CXX_ADDR_READ, addr, I2C_MEMADD_SIZE, read_buf, 1, 0xFFFF);
}

/**
 * @brief        At24Cxx任意地址连续写多个字节数据
 * @param        addr —— 写数据的地址（0-255）
 * @param        dat  —— 存放写入数据的地址
 * @retval       成功 —— HAL_OK
 */
HAL_StatusTypeDef At24Cxx_Write_Amount_Byte(uint16_t addr, uint8_t *dat, uint16_t size)
{
    uint8_t i = 0;
    uint16_t cnt = 0;         // 写入字节计数
    HAL_StatusTypeDef result; // 返回是否写入成功

    /* 对于起始地址，有两种情况，分别判断 */
    if (0 == addr % 8)
    {
        /* 起始地址刚好是页开始地址 */
        /* 对于写入的字节数，有两种情况，分别判断 */
        if (size <= 8)
        {
            // 写入的字节数不大于一页，直接写入
            result = HAL_I2C_Mem_Write(AT24Cxx_I2C, AT24CXX_ADDR_WRITE, addr, I2C_MEMADD_SIZE, dat, size, 0xFFFF);
#if SYSTEM_TIME == HAL_BARE_MACHINE
            HAL_Delay(20); // 写完八个字节（最多八个字节），延迟久一点
#elif SYSTEM_TIME == HAL_FREERTOS
            osDelay(20);
#endif

            return result;
        }
        else
        {
            // 写入的字节数大于一页，先将整页循环写入
            for (i = 0; i < size / 8; i++)
            {
                HAL_I2C_Mem_Write(AT24Cxx_I2C, AT24CXX_ADDR_WRITE, addr, I2C_MEMADD_SIZE, &dat[cnt], 8, 0xFFFF);
                // 一次写入了八个字节，延迟久一点
#if SYSTEM_TIME == HAL_BARE_MACHINE
                HAL_Delay(20); // 写完八个字节（最多八个字节），延迟久一点
#elif SYSTEM_TIME == HAL_FREERTOS
                osDelay(20);
#endif
                addr += 8;
                cnt += 8;
            }
            // 将剩余的字节写入
            result = HAL_I2C_Mem_Write(AT24Cxx_I2C, AT24CXX_ADDR_WRITE, addr, I2C_MEMADD_SIZE, &dat[cnt], size - cnt, 0xFFFF);
#if SYSTEM_TIME == HAL_BARE_MACHINE
            HAL_Delay(20); // 写完八个字节（最多八个字节），延迟久一点
#elif SYSTEM_TIME == HAL_FREERTOS
            osDelay(20);
#endif
            return result;
        }
    }
    else
    {
        /* 起始地址偏离页开始地址 */
        /* 对于写入的字节数，有两种情况，分别判断 */
        if (size <= (8 - addr % 8))
        {
            /* 在该页可以写完 */
            result = HAL_I2C_Mem_Write(AT24Cxx_I2C, AT24CXX_ADDR_WRITE, addr, I2C_MEMADD_SIZE, dat, size, 0xFFFF);
#if SYSTEM_TIME == HAL_BARE_MACHINE
            HAL_Delay(20); // 写完八个字节（最多八个字节），延迟久一点
#elif SYSTEM_TIME == HAL_FREERTOS
            osDelay(20);
#endif
            return result;
        }
        else
        {
            /* 该页写不完 */
            // 先将该页写完
            cnt += 8 - addr % 8;
            HAL_I2C_Mem_Write(AT24Cxx_I2C, AT24CXX_ADDR_WRITE, addr, I2C_MEMADD_SIZE, dat, cnt, 0xFFFF);
#if SYSTEM_TIME == HAL_BARE_MACHINE
            HAL_Delay(20); // 写完八个字节（最多八个字节），延迟久一点
#elif SYSTEM_TIME == HAL_FREERTOS
            osDelay(20);
#endif
            addr += cnt;

            // 循环写整页数据
            for (i = 0; i < (size - cnt) / 8; i++)
            {
                HAL_I2C_Mem_Write(AT24Cxx_I2C, AT24CXX_ADDR_WRITE, addr, I2C_MEMADD_SIZE, &dat[cnt], 8, 0xFFFF);
#if SYSTEM_TIME == HAL_BARE_MACHINE
            HAL_Delay(20); // 写完八个字节（最多八个字节），延迟久一点
#elif SYSTEM_TIME == HAL_FREERTOS
            osDelay(20);
#endif
                addr += 8;
                cnt += 8;
            }
            // 将剩下的字节写入
            result = HAL_I2C_Mem_Write(AT24Cxx_I2C, AT24CXX_ADDR_WRITE, addr, I2C_MEMADD_SIZE, &dat[cnt], size - cnt, 0xFFFF);
#if SYSTEM_TIME == HAL_BARE_MACHINE
            HAL_Delay(20); // 写完八个字节（最多八个字节），延迟久一点
#elif SYSTEM_TIME == HAL_FREERTOS
            osDelay(20);
#endif
            return result;
        }
    }
}

/**
 * @brief        At24Cxx任意地址连续读多个字节数据
 * @param        addr —— 读数据的地址（0-255）
 * @param        dat  —— 存放读出数据的地址
 * @retval       成功 —— HAL_OK
 */
HAL_StatusTypeDef At24Cxx_Read_Amount_Byte(uint16_t addr, uint8_t *recv_buf, uint16_t size)
{
    return HAL_I2C_Mem_Read(AT24Cxx_I2C, AT24CXX_ADDR_READ, addr, I2C_MEMADD_SIZE, recv_buf, size, 0xFFFF);
}
