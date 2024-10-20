#include <iostream>
using namespace std;


bool nand(bool a, bool b){
    if(a*b==0){
        return 1;
    }
    else return 0;
}

bool AND(bool a, bool b){
    bool ats;
    ats = nand(a, b);
    return nand(ats, ats);
}
bool OR(bool a, bool b){
    bool ats1, ats2;
    ats1 = nand(a, a);
    ats2 = nand(b, b);
    return nand(ats1, ats2);
}

bool NOR(bool a, bool b){
    bool ats;
    ats = OR(a, b);
    return nand(ats, ats);
}

bool XOR(bool a, bool b){
    bool ats1;
    bool ats2, ats3;
    ats1 = nand(a, b); 
    ats2 = nand(ats1, a);
    ats3 = nand(ats1, b);
    return nand(ats2, ats3);    
}

void H_Adder(bool a, bool b, bool &sum, bool &carry){
    sum = XOR(a, b);
    carry = AND(a, b);
}

void F_Adder(bool a, bool b, bool &sum, bool carryIn, bool &carryOut){
    bool carry2 = 0;
    bool carry1 = 0;
    H_Adder(a, b, sum, carry1);
    H_Adder(sum, carryIn, sum, carry2);
    carryOut= OR(carry1, carry2);
}
bool NOT(bool a){
    return nand(a, a);

}

void mux(bool m0, bool m1, bool sum, bool inva, bool invb, bool AxorB, bool &output){
    bool temp1;
    bool temp2;
    bool temp3;
    bool temp4;
    temp1 = AND(AND(inva, NOT(m0)), NOT(m1)); // 0 0
    temp2 = AND(AND(invb, NOT(m0)), m1); // 0 1
    temp3 = AND(AND(AxorB, m0), NOT(m1)); // 1 0
    temp4 = AND(AND(sum, m0), m1);// 1 1
    output = OR(OR(temp1, temp2), OR(temp3, temp4));
}

//void Sandauga(bool a[], bool b[], bool sandauga[]);

void oneBit_ALU(bool a, bool b, bool ena, bool enb, bool m0, bool m1, bool c1, bool &carryOut, bool &output){
    
    bool sum;
    a = AND(a, ena);
    b = AND(b, enb);
    bool inva = NOT(a);
    bool invb = NOT(b);
    bool AxorB = XOR(a, b);
    F_Adder(a, b, sum, c1, carryOut);
    mux(m0, m1, sum, inva, invb, AxorB, output);
}

void decoder38(bool s[], bool output[8]){
    output[0] = AND(AND(NOT(s[0]), NOT(s[1])), NOT(s[2])); //000
    output[1] = AND(AND(s[0], NOT(s[1])), NOT(s[2]));//100
    output[2] = AND(AND(NOT(s[0]), s[1]), NOT(s[2]));//010
    output[3] = AND(AND(s[0], s[1]), NOT(s[2])); // 110
    output[4] = AND(AND(NOT(s[0]), NOT(s[1])), s[2]); // 001
    output[5] = AND(AND(s[0], NOT(s[1])), s[2]); // 101
    output[6] = AND(AND(NOT(s[0]), s[1]), s[2]);//011
    output[7] = AND(AND(s[0], s[1]), s[2]); // 111

}

void shiftA(bool Cntr, bool a[], bool output[]){
    
        bool and1, and2, and3, and4, and5, and6, and7, and8, and9, and10, and11, and12, and13, and14;
        bool or1, or2, or3, or4, or5, or6;
        and1 = AND(a[0], NOT(Cntr));
        and2 = AND(a[1], Cntr);
        and3 = AND(a[1], NOT(Cntr));
        and4 = AND(a[2], Cntr);
        and5 = AND(a[2], NOT(Cntr));
        and6 = AND(a[3], Cntr);
        and7 = AND(a[3], NOT(Cntr));
        and8 = AND(a[4], Cntr);
        and9 = AND(a[4], NOT(Cntr));
        and10 = AND(a[5], Cntr);
        and11 = AND(a[5], NOT(Cntr));
        and12 = AND(a[6], Cntr);
        and13 = AND(a[6], NOT(Cntr));
        and14 = AND(a[7], Cntr);
        or1 = OR(and1, and4);
        or2 = OR(and3, and6);
        or3 = OR(and5, and8);
        or4 = OR(and7, and10);
        or5 = OR(and9, and12);
        or6 = OR(and11, and14);
        output[7] = and13;
        output[6] = or6;
        output[5] = or5;
        output[4] = or4;
        output[3] = or3;
        output[2] = or2;
        output[1] = or1;
        output[0] = and2;


    
}



void ALU8bit(bool decoderoutput[], bool a[], bool b[], bool s[], bool output[], bool &F, bool Cntr, bool sandauga[], bool &c1){
    
    bool carryOut{};
    bool sum_or_palyg_or_minus[8] = {0};
    bool temp1;
    bool temp2;
    bool temp3;
    bool temp4;
    if(decoderoutput[2] == 1 || decoderoutput[6] == 1 || decoderoutput[1] == 1){
        for(int i = 7; i>=0; i--){
            oneBit_ALU(a[i], b[i], 1, 1, s[1], s[2], c1, carryOut, output[i]);
            c1=carryOut;
            carryOut=0;
        }
    }
    else if(decoderoutput[4] == 1){
        for(int i = 7; i>=0; i--){
            oneBit_ALU(b[i], a[i], 1, 1, s[1], s[2], c1, carryOut, output[i]);
            c1=carryOut;
            carryOut=0;
        }
        for(int i = 7; i>=0; i--){
            if(output[i] == 0){
                oneBit_ALU(b[0], output[i], 1, 1, s[1], s[2], c1, carryOut, output[i]);
                break;
            }
            else
                oneBit_ALU(b[0], output[i], 1, 1, s[1], s[2], c1, carryOut, output[i]);
        }
    }
    else if (AND(decoderoutput[5], 1)){
        shiftA(Cntr, a, output);
    }
    else if(false){
        //Sandauga(a, b, sandauga);
        for(int i = 0; i<8; i++){
            output[i] = sandauga[i];
        }
    }
    
    temp1 = NOR(output[0], output[1]);
    temp2 = NOR(output[2], output[3]);
    temp3 = NOR(output[4], output[5]);
    temp4 = NOR(output[6], output[7]);
    F = NOR(NOR(temp1, temp2), NOR(temp3, temp4));

}


void Sandauga(bool a[], bool b[], bool sandauga[], bool decoderoutput[], bool s[], bool F, bool Cntr){
    bool desine = 0;
    bool kaire = 1;
    int k = 0;
    while(true){
        bool check = 0;
        bool c1 = 0;
        bool carryOut = 0;
        for(int i = 0; i<8; i++){
            if(b[i]==1){
                check = 1;
                continue;
            }
        }
        if(check == 0){
            break;
        }
        if(b[7] == 0){
            shiftA(desine, b, b);
            shiftA(kaire, a, a);
        }
        else{
            
            ALU8bit(decoderoutput, sandauga, a, s, sandauga, F, Cntr, {0}, c1);
            shiftA(kaire, a, a);
            shiftA(desine, b, b);
            k++;




            // oneBit_ALU(sandauga[15], a[7], 1, 1, 1, 1, c1, carryOut, sandauga[15]);
            // c1=carryOut;
            // carryOut=0;
            // oneBit_ALU(sandauga[14], a[6], 1, 1, 1, 1, c1, carryOut, sandauga[14]);
            // c1=carryOut;
            // carryOut=0;
            // oneBit_ALU(sandauga[13], a[5], 1, 1, 1, 1, c1, carryOut, sandauga[13]);
            // c1=carryOut;
            // carryOut=0;
            // oneBit_ALU(sandauga[12], a[4], 1, 1, 1, 1, c1, carryOut, sandauga[12]);
            // c1=carryOut;
            // carryOut=0;
            // oneBit_ALU(sandauga[11], a[3], 1, 1, 1, 1, c1, carryOut, sandauga[11]);
            // c1=carryOut;
            // carryOut=0;
            // oneBit_ALU(sandauga[10], a[2], 1, 1, 1, 1, c1, carryOut, sandauga[10]);
            // c1=carryOut;
            // carryOut=0;
            // oneBit_ALU(sandauga[9], a[1], 1, 1, 1, 1, c1, carryOut, sandauga[9]);
            // c1=carryOut;
            // carryOut=0;
            // oneBit_ALU(sandauga[8], a[0], 1, 1, 1, 1, c1, carryOut, sandauga[8]);
            // c1=carryOut;
            // carryOut=0;
            
        }

    }
}

bool check(bool a[], bool b[], bool c1, bool ALUoutput[]){
    bool check = 0;
    if (AND(c1, 1)){
        check = 1;
    }
    else if(AND(!AND(a[0], 1), !AND(b[0], 1)), AND(ALUoutput[0], 1)){
        check = 1;
    }
    return check;
}


int main(){
    int pasirinkimas;
    cout << "ar norite realizuoti: [1] bito ALU ar [8] bitu ALU: "; cin >> pasirinkimas;
    if(pasirinkimas == 1){
        bool a, b, ena, enb;
        int k{};
        bool s1, s2, s3;
        bool c1 = 0;
        char tempa, tempb, tempc;
        bool carryOut = 0;
        bool sum = 0;
        bool carry = 0;
        bool carryIn = 0;
        bool carryonward = 0;
        bool output;
        bool m[3];
        cout << "iveskite oradinius duom: "; cin >> a >> b;
        cout << "iveskite ena, enb: "; cin >> ena >> enb;
        // cout << "iveskite ka programa turetu daryt"; cin >> s1 >> s2;
        cout << "iveskite mux komb: " ;
        for(int i = 0; i<3; i++){
            cin >> tempa;
            if(i == 0){
                continue;
            }
            m[k]=int(tempa)-48;
            k++;
        }
        oneBit_ALU(a,b,ena, enb, m[0],m[1],c1,carryOut, output);
        if(m[0] == 0 && m[1]==0){
            cout << "A bito [" <<a<<"] inversija yra " << output << endl;
        }
        if(m[0] == 0 && m[1]==1){
            cout << "B bito [" <<b<<"] inversija yra " << output << endl;
        }
        if(m[0] == 1 && m[1]==0){
            if(output == 1 ){
                 cout << "A nelygu B" << endl;
                 cout << "A = " << a << endl;
                 cout << "B = " << b << endl;
            }
            else{
                cout << "A lygu B" << endl;
                 cout << "A = " << a << endl;
                 cout << "B = " << b << endl;
            }
        }
        if(m[0] == 1 && m[1]==1){
            cout << "A[" <<a<<"] + B[" << b <<"] + " <<c1 <<  " = "<< output << endl;
            cout << "Kelinys: " << carryOut;
        }
    }
    if(pasirinkimas == 8){
        bool decoderOutput[8] = {0};
        bool ALUoutput[8] = {0};
        bool s[3];
        bool a[8], b[8];
        bool c1 {};
        bool F, Cntr, sandauga[16] = {0};
        int k = 0;
        cout << "iveskite a";
        for(int i = 0; i<8; i++){
            char tempa;
            cin >> tempa;
            a[k]=int(tempa)-48;
            k++;
        }
        k = 0;
        cout << "iveskite b";
        for(int i = 0; i<8; i++){
            char tempa;
            cin >> tempa;
            b[k]=int(tempa)-48;
            k++;
        }
        k = 0;
        cout << "iveskite s1, s2, s3: ";
        for(int i = 0; i<3; i++){
            char tempa;
            cin >> tempa;
            s[k]=int(tempa)-48;
            k++;
        }
        cout << "iveskite Cntr: "; cin >> Cntr;

        decoder38(s, decoderOutput);
        ALU8bit(decoderOutput, a, b, s, ALUoutput, F, Cntr, sandauga, c1);
        if(decoderOutput[4] == 1){
            for(int i = 0; i<8; i++){
                cout << a[i];
            }
            cout << " neigiamas bus: ";
            for(int i = 0; i<8; i++){
                cout << ALUoutput[i];
            }
            return 0;
        }
        // for(int i = 0; i<8; i++){
        //     cout << ALUoutput[i];
        // }

        else if(decoderOutput[5] == 1){
            for(int i = 0; i<8; i++){
                cout << a[i];
            }
            if(Cntr == 0){
                cout << " Shift i desine: ";
            }
            else if(Cntr == 1){
                cout << " Shift i kaire: ";
            }
            for(int i = 0; i<8; i++){
                cout << ALUoutput[i];
            }
        }
        else if(decoderOutput[1] == 1){
            Sandauga(a, b, sandauga, decoderOutput, s, F, Cntr);
            cout << "A ir B sandauga = ";
            for(int i = 0; i<16; i++){
                cout << sandauga[i];
            }
        }
        else if(decoderOutput[6]==1){
            cout << "A ir B suma: ";
            for(int i = 0; i<8; i++){
                cout << ALUoutput[i];
            }
            if(check(a, b, c1, ALUoutput)){
                cout << "\nskaicius overflowino\n";
                cout << "c1: " << c1;
            }
        }
        else if(decoderOutput[2] == 1){
            if(F == 1){
                cout << "A ir B lygus" << endl;
            }
            else cout << "A ir B nelygus" << endl;
        }
    } 
}