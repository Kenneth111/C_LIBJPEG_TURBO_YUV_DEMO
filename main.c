#include "tj_test.h"
#include "yuv_test.h"

int main()
{
    tj_test();
//    yuv_test("enc_yuv.yuv", 0);
    yuv_test("d:\\yuv420.yuv", 1);
}
