// public static void CalulateTable_CRC8()
// {
//     const byte generator = 0x1D;
//     crctable = new byte[256];
//     /* iterate over all byte values 0 - 255 */
//     for (int divident = 0; divident < 256; divident++)
//     {
//         byte currByte = (byte)divident;
//         /* calculate the CRC-8 value for current byte */
//         for (byte bit = 0; bit < 8; bit++)
//         {
//             if ((currByte & 0x80) != 0)
//             {
//                 currByte <<= 1;
//                 currByte ^= generator;
//             }
//             else
//             {
//                 currByte <<= 1;
//             }
//         }
//         /* store CRC value in lookup table */
//         crctable[divident] = currByte;
//     }
// } 



// public static byte Compute_CRC8(byte[] bytes)
// {
//     byte crc = 0;
//     foreach (byte b in bytes)
//     {
//         /* XOR-in next input byte */
//         byte data = (byte)(b ^ crc);
//         /* get current CRC value = remainder */
//         crc = (byte)(crctable[data]);
//     }

//     return crc;
// } 


void calcula_tabela_crc(char *table){

    char base = 0x1D;
    char byteAtual;
    printf("Tamanho do arquivo:");
    printf("%d\n",strlen(table));
    int max = 256;
    int dividendo;

    for(char i = 0 ; i < max ;i++){
        byteAtual = i;
        /* calculate the CRC-8 value for current byte */
        for (int bit = 0; bit < 8; bit++)
        {
            if ((byteAtual & 0x80) != 0)
            {
                byteAtual <<= 1;
                byteAtual ^= base;
            }
            else
            {
                byteAtual <<= 1;
            }
        }
        /* store CRC value in lookup table */
        table[dividendo] = byteAtual;

}