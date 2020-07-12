#include <iostream>
#include "BigNumber.h"


int main() {

    BigNumber a("100.0");
    BigNumber b("25.4");
    BigNumber c = a.divide(b);

    //prints 3.937007874015748031496062992125984251968503937007874015748031496062992125984251968503937007874015748031496
    std::cout << c.toString() << std::endl;

    //prints 265252859812191058636308480000000.0
    std::cout << bFactorial(30).toString() << std::endl;

    //prints 0.896088024556635616775481910743818683597567836174694146764772389414193493857091339733456465089726068
    std::cout << bLog(2.45).toString() << std::endl;

    //prints 125.4
    std::cout << a.add(b).toString() << std::endl;

    //prints 1.414213562373095048801688724209698078569671875376948073176679737990732478462107038850387534327641573
    std::cout << bSqrt(2).toString() << std::endl;

    return 0;


}
