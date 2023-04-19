#include <iostream>
#include <random>
#include <bitset>
#include <immintrin.h>

    /* function to show bytes in memory, from location start to start+n*/
void show_mem_rep(char *start, int n)
   {
      int i;
      for (i = 0; i < n; i++)
         printf(" %.2x", start[i]);
      printf("\n");
   }

int main() {
   // std::random_device rd;
   // std::mt19937_64 gen(rd());
   // std::uniform_int_distribution<uint64_t> dis;
   // uint64_t random_number = dis(gen);
   // std::cout << "Random number: " << random_number << std::endl;
   // uint8_t test = 91;
   // std::bitset<8> x(test);
   // std::cout << "test      = " << test        << std::endl;
   // std::cout << "x         = " << x           << std::endl;
   // std::bitset<8> y(test >> 5);
   // std::cout << "y    >> 5 = " << y           << std::endl;
   // for(uint32_t i=0; i<8; i++) 
   //    std::cout << "bool test[" << i << "] = " << (bool)((test >> (7 - i)) & 1)  << std::endl;
   // std::bitset<32> h(9499938);
   // std::bitset<32> j(7);
   // std::cout << h << std::endl;
   // std::cout << j << std::endl;
   // uint8_t a[3];
   // a[0]=0;
   // a[1]=7;
   // a[2]=64;
   // std::bitset<8> a0(a[0]);
   // std::bitset<8> a1(a[1]);
   // std::bitset<8> a2(a[2]);
   // std::cout << a[0] << " " << a[1] << " " << a[2] <<  std::endl;
   // std::cout << a0 << " " << a1 << " " << a2 << std::endl;
   // std::cout << (a[1] % 2) << std::endl;
   uint32_t i = 0x01234567;
   std::cout << "i = " << i << " ";
   std::bitset<32> l(i);
   std::cout << "of " << sizeof(i) << "bytes" << std::endl;
   std::cout << "(i)_2 = " << l << std::endl;
   show_mem_rep((char *)&i, sizeof(i));
   uint8_t c[]={67,45,23,1};
   for (int i = 0; i<4; i++) {
      std::bitset<8> p(c[i]);
      std::cout << (int)c[i] << " = " << p << std::endl;
   }
   unsigned short us[3] = {0, 0xFF, 0xFFFF};
   unsigned short usr;
   unsigned int   ui[4] = {0, 0xFF, 0xFFFF, 0xFFFFFFFF};
   unsigned int   uir;

   for (int i=0; i<3; i++) {
     usr = __lzcnt16(us[i]);
     std::cout << "__lzcnt16(0x" << std::hex << us[i] << ") = " << std::dec << usr << std::endl;
   }

   for (int i=0; i<4; i++) {
     uir = __lzcnt32(ui[i]);
     std::cout << "__lzcnt32(0x" << std::hex << ui[i] << ") = " << std::dec << uir << std::endl;
   }

   uint8_t q[2] = {0x1, 0x00};

   std::cout << "q[0]= 0x" << std::hex << (int)q[0] << std::endl;
   std::cout << "q[1]= 0x" << std::hex << (int)q[1] << std::endl;

   uint16_t *w;

   w = (uint16_t *)(q);

   std::cout << "*w = 0x" << std::hex << *w << std::endl;

   std::bitset<16> t(*w);

   std::cout << "w in binary is " << t << std::endl;

   std::cout << "_lzcnt_u32(0x" << std::hex << (int)q[0] << ") = " << std::dec <<_lzcnt_u32(q[0]) << std::endl;
   std::cout << "_lzcnt_u64(0x" << std::hex << (int)q[0] << ") = " << std::dec <<_lzcnt_u64(q[0]) << std::endl;
   
   std::cout << "_lzcnt_u32(0x" << std::hex << (int)q[1] << ") = " << std::dec <<_lzcnt_u32(q[1]) << std::endl;
   std::cout << "_lzcnt_u64(0x" << std::hex << (int)q[1] << ") = " << std::dec <<_lzcnt_u64(q[1]) << std::endl;

   std::cout << "_lzcnt_u32(0x" << std::hex << w[0] << ") = " << std::dec <<_lzcnt_u32(w[0]) << std::endl;
   std::cout << "_lzcnt_u64(0x" << std::hex << w[0] << ") = " << std::dec <<_lzcnt_u64(w[0]) << std::endl;


   return 0;
}
