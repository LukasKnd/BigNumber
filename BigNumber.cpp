#include "BigNumber.h"

using namespace std;


string convertDoubleToString(double num) {
    return to_string(num);
}

BigNumber::BigNumber() {
        this->nums = {0,0};
        this->sign = 1;
        this->scale = 1;
}


BigNumber::BigNumber(const std::string &num) {
        try {
            process_string(num);
        } catch(int e) {
            cout << "EROR CREATING NUMBER. STRING: \" "<< num << "\" IS INVALID" << endl;
            exit(1);
        }
    }

BigNumber::BigNumber(long num) {
        if(num == 0) {
            this->nums = {0,0};
            this->sign = 1;
            this->scale = 1;
        } else {
            this->sign = 1;
            if(num < 0) {
                num *= -1;
                this->sign = -1;
            }

            vector<int> res;
            while(num != 0) {
                    this->nums.push_back(num%10);
                num /= 10;
            }
            reverse(this->nums.begin(),this->nums.end());
            this->nums.push_back(0);
            this->scale = 1;
        }

    }

void BigNumber::process_string (const std::string &s) {

        regex reg("^(\\+|-)?[[:digit:]][[:digit:]]*\\.[[:digit:]]+$");

        int i = 0;
        this->sign = 1;
        if (s[i] == '-' || s[i] == '+') {
            if (s[i] == '-') {
                this->sign = -1;
            }
            i++;
        }

        if((s[i] == '0' && s[i+1] != '.') || s.compare("-0.0") == 0 || !regex_match(s,reg)) {
            throw 0;
        }

        int len = s.length();

        while ('0' <= s[i] && s[i] <= '9') {
            this->nums.push_back(s[i]-'0');
            i++;
        }
        int dec = this->nums.size();
        i++;
        while (i < len) {
            this->nums.push_back(s[i]-'0');
            i++;
        }
        this->scale = nums.size() - dec;

        truncZero();
    }


string BigNumber::toString() {
        std::string res;
        if(this->sign == -1)
            res += "-";
        for(int i = 0; i < this->nums.size(); i++) {
            if(i == this->nums.size() - this->scale)
                res += ".";
            res+= to_string(this->nums[i]);
        }
        return res;

    }

void BigNumber::truncZero() {
        int i = this->nums.size() - 1;
        for(; i != this->nums.size() - this->scale; i--) {
            if(this->nums[i] != 0)
                break;
        }
        this->scale -= this->nums.size() - i - 1;
        this->nums.resize(i+1);
    }

void BigNumber::removeLeadingZero() {
        int i;
        for(i = 0; i < this->nums.size() - this->scale - 1; i++) {
            if(this->nums[i] != 0)
                break;
        }
        if(i != 0 && this->nums[0] == 0) {
            this->nums.erase(this->nums.begin(),this->nums.begin()+i);
        }
    }

int BigNumber::getLarger(const vector<int> &a, const vector<int> &b, const int &scaleA, const int &scaleB) {
        int aDec = a.size() - scaleA;
        int bDec = b.size() - scaleB;

        if(aDec > bDec) {
            return 1;
        }
        if(bDec > aDec) {
            return -1;
        }

        int i;
        int l1 = a.size();
        int l2 = b.size();

        for(i = 0; i < l1 && i < l2; i++) {
            if(a[i] > b[i]) {
                return 1;
            }
            if(b[i] > a[i]) {
                return -1;
            }
        }
        while(i < l1 && i < l2) {
            if(a[i] > b[i]) {
                return 1;
            }
            if(b[i] > a[i]) {
                return -1;
            }
        }
        if(l1 == l2) {
            return 0;
        }

        return l1 > l2 ? 1 : -1;
    }

int BigNumber::compareTo (const BigNumber &rhs) {
        if(this->sign != rhs.sign)
            return (this->sign - rhs.sign) / 2;

        int c = (1 - 2 * (this->sign < 0));

        return getLarger(this->nums, rhs.nums, this->scale, rhs.scale) * c;
    }

void BigNumber::sub_pos(const BigNumber &larger, const BigNumber &smaller, BigNumber &res) {
        res = larger;
        if(larger.scale < smaller.scale) {
            int i = larger.scale;
            while(i != smaller.scale) {
                res.nums.push_back(0);
                i++;
            }
            res.scale = smaller.scale;
        }
        int smallerPos = smaller.nums.size()-1;
        int resPos = res.nums.size() -1 - (res.scale-smaller.scale);
        int memory = 0;
        int x;
        for(; smallerPos >= 0; smallerPos--, resPos-- ) {
            x = res.nums[resPos] - smaller.nums[smallerPos] - memory;
            if(x < 0) {
                memory = 1;
                res.nums[resPos] = 10-(-1*x);
            } else {
                res.nums[resPos] = x;
                memory = 0;
            }
        }

        while(memory != 0 && resPos > -1) {
            x = res.nums[resPos] - memory;

            if(x < 0) {
                memory = 1;
                res.nums[resPos] = 10-(-1*x);
            } else {
                res.nums[resPos] = x;
                memory = 0;
            }
            resPos--;
        }
    }

BigNumber BigNumber::add(const BigNumber &rhs) {
        int bigger = getLarger(this->nums, rhs.nums, this->scale, rhs.scale);
        BigNumber summed;

        if(this->sign == rhs.sign) {
            if(bigger > 0)
                sum_pos(*this,rhs,summed);
            else
                sum_pos(rhs,*this,summed);
        } else if(this->sign != rhs.sign) {
            if(bigger > 0)
                sub_pos(*this,rhs,summed);
            if (bigger < 0)
                sub_pos(rhs,*this,summed);
        }
        summed.removeLeadingZero();
        summed.truncZero();
        return summed;
    }

void BigNumber::sum_pos(const BigNumber &larger, const BigNumber &smaller, BigNumber &res) {
        res = larger;
        if(larger.scale < smaller.scale) {
            int i = larger.scale;
            while(i != smaller.scale) {
                res.nums.push_back(0);
                i++;
            }
            res.scale = smaller.scale;
        }
        int smallerPos = smaller.nums.size()-1;
        int resPos = res.nums.size() -1 - (res.scale-smaller.scale);

        int memory = 0;
        for(; smallerPos >= 0; smallerPos--, resPos-- ) {
            int sum = res.nums[resPos] + smaller.nums[smallerPos] + memory;
            res.nums[resPos] = sum % 10;
            memory = sum / 10;
        }

        while(memory != 0 && resPos > -1) {
            int sum = res.nums[resPos] + memory;
            res.nums[resPos] = sum % 10;
            memory = sum / 10;
            resPos--;
        }
        if(memory > 0) {
            res.nums.emplace(res.nums.begin(), memory);
        }

    }

BigNumber BigNumber::subtract(const BigNumber &rhs) {
        BigNumber cpy;
        cpy = rhs;
        cpy.sign *= -1;
        cpy = this->add(cpy);
        return cpy;
    }

BigNumber BigNumber::multiply(const BigNumber &rhs, int rounding) {
        BigNumber res;
        if(res.compareTo(*this) == 0 || res.compareTo(rhs) == 0) {
            return res;
        }
        if(this->sign != rhs.sign) {
            res.sign = -1;
        }

        res.scale = this->scale + rhs.scale;
        int scl = res.scale;
        for(int i = this->nums.size() - 1; i >= 0; i--) {
            if(this->nums[i] != 0) {
                BigNumber current;
                current.nums.clear();
                current.scale = scl;
                current.sign = res.sign;
                int memory = 0;
                for(int j = rhs.nums.size() - 1; j >= 0; j--) {
                    int mult = (this->nums[i] * rhs.nums[j]) + memory;
                    current.nums.insert(current.nums.begin(),1,mult %10);
                    memory = mult/10;
                }
                if(memory != 0) {
                    current.nums.insert(current.nums.begin(),1,memory);
                }
                int extra = scl - current.nums.size() + 1;
                if(extra > 0) {
                    current.nums.insert(current.nums.begin(),extra,0);
                }
                res = res.add(current);
            }
            scl--;
        }

        return res.round(rounding);
    }

BigNumber BigNumber::ceil() {
        BigNumber res = *this;

        if(res.scale == 1 && res.nums[res.nums.size() - 1] == 0) {
            return res;
        }
        res.nums.resize(res.nums.size() - res.scale + 1);
        res.nums[res.nums.size() - 1] = 0;
        res.scale = 1;

        BigNumber one("1.0");
        res = res.add(one);

        return res;
    }

BigNumber BigNumber::times10(int times) {
        BigNumber res = *this;
        res.scale = res.scale - times;
        while(res.scale < 1) {
            res.nums.push_back(0);
            res.scale++;
        }
        res.removeLeadingZero();
        return res;
    }

BigNumber BigNumber::mod(const BigNumber &rhs) {

        BigNumber zero;

        BigNumber a = *this;
        BigNumber b = rhs;

        a.sign = 1;
        b.sign = 1;

        if(a.compareTo(rhs) <= 0) {
            return *this;
        }

        if(zero.compareTo(rhs) == 0) {
            //division by zero
            throw 1;
        }

        int curMaxScl = max(a.scale,b.scale);
        a = a.times10(curMaxScl);
        b = b.times10(curMaxScl);
        int alen = a.nums.size() - 1;
        int blen = b.nums.size() - 1;


        BigNumber current;
        current.nums.clear();
        if(alen >= blen) {
            current.nums.insert(current.nums.begin(),a.nums.begin(),a.nums.begin()+blen);
            current.nums.push_back(0);
            if(current.compareTo(b) < 0 && alen > blen) {
                current.nums.insert(current.nums.end()-1,a.nums[blen]);
            }
        } else {
            current = a;
        }

        int lastTakenDigitPos = current.nums.size() - 2;

        while(1) {
            for(int i = 0; i < 9; i++) {
                if(current.compareTo(b) >= 0) {
                    current = current.subtract(b);
                } else {
                    break;
                }
            }

            if(lastTakenDigitPos+1 >= alen) {

                break;

            } else {
                current.nums.insert(current.nums.end()-1,a.nums[++lastTakenDigitPos]);
            }
            current.removeLeadingZero();
        }

            current.scale += curMaxScl;
            while(current.nums.size() <= current.scale) {
                current.nums.insert(current.nums.begin(), 0);
            }

        current.truncZero();

        if(current.compareTo(zero) != 0)
            current.sign = this->sign;

        return current;

    }

BigNumber BigNumber::divide(const BigNumber &rhs, int maxScale) {

        BigNumber zero;

        if(this->compareTo(zero) == 0) {
            return *this;
        }
        if(zero.compareTo(rhs) == 0) {
            //division by zero
            throw "can't divide by zero";
        }

        BigNumber a = *this;
        BigNumber b = rhs;
        a.sign = 1;
        b.sign = 1;

        if(a.compareTo(zero) == 0)
            return a;
        if(b.compareTo(zero) == 0) {
            return zero;
        }

        int curMaxScl = max(a.scale,b.scale);
        a = a.times10(curMaxScl);
        b = b.times10(curMaxScl);
        int alen = a.nums.size() - 1;
        int blen = b.nums.size() - 1;

        BigNumber current;
        current.nums.clear();
        if(alen >= blen) {
            current.nums.insert(current.nums.begin(),a.nums.begin(),a.nums.begin()+blen);
            current.nums.push_back(0);
            if(current.compareTo(b) < 0 && alen > blen) {
                current.nums.insert(current.nums.end()-1,a.nums[blen]);
            }
        } else {
            current = a;
        }

        int lastTakenDigitPos = current.nums.size() - 2;

        BigNumber result;

        result.nums.erase(result.nums.begin(),result.nums.begin()+1);


        bool fracPart = false;
        while(1) {
            int fits = 0;
            for(int i = 0; i < 9; i++) {
                if(current.compareTo(b) >= 0) {
                    fits++;
                    current = current.subtract(b);
                } else {
                    break;
                }
            }

            result.nums.insert(result.nums.end()-1,fits);
            if(fracPart) {
                result.scale++;
            }

            if(lastTakenDigitPos+1 > alen) {
                fracPart = true;
                if(current.compareTo(zero) == 0) {
                    break;
                }
                current.nums.insert(current.nums.end()-1,0);
            } else {
                current.nums.insert(current.nums.end()-1,a.nums[++lastTakenDigitPos]);
            }

            current.removeLeadingZero();

            if(result.scale > maxScale) {
                break;
            }

        }

        result.scale++;
        result.removeLeadingZero();
        result.truncZero();

        if(this->sign != rhs.sign) {
            result.sign = -1;
        }
        return result.round(maxScale);

    }

BigNumber BigNumber::power(int pow) {
        BigNumber output(1);

        while(pow>0){
             output = this->multiply(output);
             pow--;
        }

        return output;
    }

BigNumber BigNumber::round(int scale) {

    BigNumber rounded = *this;

    if(scale < this->scale) {

        rounded.nums.erase(rounded.nums.end() - rounded.scale + scale +1,rounded.nums.end());
        rounded.scale = scale;

        int last = rounded.nums[rounded.nums.size()-1];

        if(last >=5) {
            int i = rounded.nums.size()-2;
            while(++rounded.nums[i] == 10) {
                rounded.nums[i] = 0;

                i--;
                if(i == -1)
                    break;
            }
            if(i == -1) {
                rounded.nums.insert(rounded.nums.begin(), 1);
            }
        }
        rounded.nums.pop_back();
        rounded.truncZero();
    }

    return rounded;

}

BigNumber BigNumber::abs(const BigNumber &rhs) {

    BigNumber subtraction = this->subtract(rhs);
    subtraction.sign = 1;

    return subtraction;

}

bool isprime(BigNumber n) {

    if(n.compareTo(BigNumber(1)) <= 0)
        return false;

    if(n.compareTo(BigNumber(3)) <= 0)
        return true;


    if(n.mod(BigNumber(2)).compareTo(BigNumber(0)) == 0 || n.mod(BigNumber(3)).compareTo(BigNumber(0)) == 0)
        return false;


    BigNumber i(5);
    for( ;i.multiply(i).compareTo(n) <= 0;i=i.add(BigNumber(6))) {
        if(n.mod(i).compareTo(BigNumber(0)) == 0 || n.mod(i.add(BigNumber(2))).compareTo(BigNumber(0)) == 0)
            return false;
    }

    return true;
}

BigNumber bFactorial(int n){
    BigNumber res;

    static map<int, BigNumber> m = {{0,BigNumber(1)},{1, BigNumber(1)}, {2, BigNumber(2)}, {3, BigNumber(6)}, {4, BigNumber(24)}};

    auto it = m.find(n);
    int i = 1;
    if (it != m.end()) {
        res = get<1>(*it);
        i = get<0>(*it) + 1;
    } else {
        auto rit = m.rbegin();
        res = get<1>(*rit);
        i = get<0>(*rit) + 1;
    }

    for( ; i <=n; ++i)
    {
        res = res.multiply(i);
        m.insert ( std::pair<int,BigNumber>(i,res) );
    }
    return res;

}
BigNumber bPrime(BigNumber n){
    BigNumber one("1.0");
    BigNumber two("2.0");

    if(n.scale != 1 && n.nums[n.nums.size() -1] != 0) {
        n = n.ceil();
    } else {
        n = n.add(one);
    }

    bool found = false;

    while (!found) {
        if (isprime(n)) {
            found = true;
            return n;
        }
        n = n.add(one);
    }
}
BigNumber bPow(double d, int p){
    BigNumber rad  = convertDoubleToString(d);
    rad = rad.power(p).round(99);
    return rad;
}

BigNumber bLog(double d){
    BigNumber x = convertDoubleToString(d);

    BigNumber result;
    BigNumber z("1.82");
    BigNumber v("0.18");

    BigNumber ln10("2.302585092994045684017991454684364207601101488628772976033327900967572609677352480235997205089598298341967784042286248633409525465");

    while(x.compareTo(z) > 0) {
        result = result.add(BigNumber(1));
        x = x.multiply(BigNumber("0.1"));
    }

    while(x.compareTo(v) < 0) {
        result = result.subtract(BigNumber(1));
        x = x.multiply(BigNumber(10));
    }

    result = result.multiply(ln10);

    BigNumber result2;
    BigNumber h("1.34");
    BigNumber l("0.67");

    BigNumber ln2("0.6931471805599453094172321214581765680755001343602552541206800094933936219696947156058633269964186875420014810205706857336855202358");

    while(x.compareTo(h) > 0) {
        result2 = result2.add(BigNumber(1));
        x = x.multiply(BigNumber("0.5"));
    }

    while(x.compareTo(l) < 0) {
        result2 = result2.subtract(BigNumber(1));
        x = x.multiply(BigNumber(2));
    }

    result2 = result2.multiply(ln2);

    x = x.subtract(BigNumber(1));

    BigNumber oldx = x;
    BigNumber prec;
    BigNumber t = x;
    BigNumber i(1);
    BigNumber semn("-1.0");

    BigNumber lll;

    BigNumber ACCURACY("0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");

    do {
        prec=x;
        t=t.multiply(oldx);
        i = i.add(BigNumber(1));
        lll = t.divide(i);
        x = prec.add(lll.multiply(semn));
        semn.sign *= -1;
        lll.sign = 1;
    } while(lll.compareTo(ACCURACY) > 0);

    return result.add(result2).add(x).round(99);
}

BigNumber bSin(double d){

    BigNumber epsilon("0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
    BigNumber rad  = convertDoubleToString(d);
    BigNumber pi("3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148");

    rad = rad.mod(pi.multiply(BigNumber(2)));

    BigNumber c;
    BigNumber result;
    int y = 0;

    BigNumber b = rad;

    while(1)
    {
        c = bFactorial((2*y)+1);
        if(y %2 == 0) {
            b.sign = 1;
        } else {
           b.sign = -1;
        }

        BigNumber div = b.divide(c,120);

        bool changed = false;
        if(div.sign == -1) {
            div.sign = 1;
            changed = true;
        }

        if(div.compareTo(epsilon) <= 0) {
            break;
        }

        if(changed) {
            div.sign = -1;
        }

        result = result.add(div);
        y++;

        b = b.multiply(rad).multiply(rad);

    }

    if(rad.sign == -1) {
        result.sign = result.sign * -1;
    }

   return result.round(99);
}


BigNumber bSqrt(double d) {
    BigNumber x = convertDoubleToString(d);
    BigNumber epsilon("0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");

    BigNumber g(1), ng;
    while(1) {
        ng = x.divide(g).add(g).multiply(BigNumber("0.5"));
        if(ng.abs(g).compareTo(epsilon) < 0) {
            break;
        }
        g = ng;
    }
    return g.round(99);
}

BigNumber bPi(int n){
    BigNumber pi("3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196");
    if(n > 99) {
        return pi.round(99);
    }
    if(n == 0) {
        return BigNumber(3);
    }
    if(n < 0) {
        return pi.round(99);
    }
    return pi.round(n);
}




