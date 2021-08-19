#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PUTC_COLOR(color, chr) \
    printf("\x1b[38;5;%dm%c\e[0m", color, chr)

enum Belt {
    NOP,
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_R,
    DIAGONAL_L,
    _MAX_BELT
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Surface {
    uint8_t millions;
    uint8_t thousands;
    uint8_t hundreds;
};

struct Gravity {
    uint8_t millions;
    uint8_t thousands;   
};
struct Years {
    uint8_t millions;
    uint8_t thousands;
};

struct Name {
    uint8_t sylab_a;
    uint8_t sylab_b;
    uint8_t sylab_c;
    uint8_t id_x;
    uint8_t id_y;
};

struct Planet {
    uint8_t galaxy;
    uint8_t solar_system;
    struct Color color;
    struct Surface surface;
    struct Gravity gravity;
    struct Years years;
    struct Name name;
    struct Color color_belt;
};

struct ASCIIArt {
    char water_char;
    char continent_char;
    char belt_char;
    _Bool water;
    enum Belt belt;
};

int rgb_to_x256(struct Color color) {
    uint8_t r = color.r;
    uint8_t g = color.g;
    uint8_t b = color.b;

    // Calculate the nearest 0-based color index at 16 .. 231
#   define v2ci(v) (v < 48 ? 0 : v < 115 ? 1 : (v - 35) / 40)
    int ir = v2ci(r), ig = v2ci(g), ib = v2ci(b);   // 0..5 each
#   define color_index() (36 * ir + 6 * ig + ib)  /* 0..215, lazy evaluation */

    // Calculate the nearest 0-based gray index at 232 .. 255
    int average = (r + g + b) / 3;
    int gray_index = average > 238 ? 23 : (average - 3) / 10;  // 0..23

    // Calculate the represented colors back from the index
    static const int i2cv[6] = {0, 0x5f, 0x87, 0xaf, 0xd7, 0xff};
    int cr = i2cv[ir], cg = i2cv[ig], cb = i2cv[ib];  // r/g/b, 0..255 each
    int gv = 8 + 10 * gray_index;  // same value for r/g/b, 0..255

    // Return the one which is nearer to the original input rgb value
#   define dist_square(A,B,C, a,b,c) ((A-a)*(A-a) + (B-b)*(B-b) + (C-c)*(C-c))
    int color_err = dist_square(cr, cg, cb, r, g, b);
    int gray_err  = dist_square(gv, gv, gv, r, g, b);

    return color_err <= gray_err ? 16 + color_index() : 232 + gray_index;
}

void print_planet(struct ASCIIArt art, char *part, uint8_t color_continent, uint8_t color_belt) {
    char *ptr = part;

    for (; *ptr; ptr++) { 
        if (!art.water) {
            if (*ptr == '~')
                PUTC_COLOR(color_continent, art.continent_char); 
            else if (*ptr == '$') 
                PUTC_COLOR(color_belt, art.belt_char);
            else
                putchar(*ptr);

        } else {
            if (*ptr == '~') {
                if (rand() % 2)
                    putchar(art.water_char);
                else
                    PUTC_COLOR(color_continent, art.continent_char);
                
            }
            else if (*ptr == '$')
                PUTC_COLOR(color_belt, art.belt_char);
                                
            else 
                putchar(*ptr);
        }
    }
}

void print_full_planet(struct ASCIIArt art, uint8_t color_belt, uint8_t color_continent) {
    char *planet[][7] = {
        [NOP] = {
            "   ,~~~~~~~.\n",
            " ,~~~~~~~~~~~.\n",
            " ~~~~~~~~~~~~~\n",
            " ~~~~~~~~~~~~~\n",
            " ~~~~~~~~~~~~~\n",
            " '~~~~~~~~~~~'\n",
            "   '~~~~~~~'\n"
        },
 
        [DIAGONAL_R] = {
            "   ,~~~~~~$.\n",
            " .~~~~~~~$$~~.\n",
            " ~~~~~~~$$~~~~\n",
            " ~~~~~~$$~~~~~\n",
            " ~~~~~$$~~~~~~\n",
            " '~~~$$~~~~~~'\n",
            "   '$$~~~~~'\n"
        },

        [DIAGONAL_L] = {
            "   ,$$~~~~~.\n",
            " .~~~$$~~~~~~.\n",
            " ~~~~~$$~~~~~~\n",
            " ~~~~~~$$~~~~~\n",
            " ~~~~~~~$$~~~~\n",
            " '~~~~~~~$$~~'\n",
            "   '~~~~~~$'\n"
        },    
        
        [HORIZONTAL] = {
            "   ,~~~~~~~.\n",
            " .~~~~~~~~~~~.\n",
            " ~~~~~~~~~~~~~\n",
            " $$$$$$$$$$$$$\n",
            " ~~~~~~~~~~~~~\n",
            " '~~~~~~~~~~~'\n",
            "   '~~~~~~~'\n"
        },

        [VERTICAL] = {
            "   ,~~~$~~~.\n",
            " .~~~~~$$~~~~.\n",
            " ~~~~~$$~~~~~~\n",
            " ~~~~~$$~~~~~~\n",
            " ~~~~~$$~~~~~~\n",
            " '~~~~~$$~~~~'\n",
            "   '~~~$~~~'\n",
        }
   };

    for (int i = 0; i < 7; i++)
        print_planet(art, planet[art.belt][i], color_belt, color_continent);
};

void print_color(struct Color color) {
    printf("%02X%02X%02X",
            color.r,
            color.g,
            color.b);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <GPG_ID>\n", argv[0]);
        return -1;
    }

    char *syllabe[256] = {
		[0] = "wew",		[1] = "oru",		[2] = "xed",		[3] = "ifu",		[4] = "zav",
		[5] = "isi",		[6] = "tif",		[7] = "xeg",		[8] = "ume",		[9] = "ofu",
		[10] = "yaz",		[11] = "imu",		[12] = "iqi",		[13] = "abi",		[14] = "izo",
		[15] = "oza",		[16] = "ipo",		[17] = "iki",		[18] = "oki",		[19] = "ede",
		[20] = "jet",		[21] = "uzu",		[22] = "vug",		[23] = "rak",		[24] = "eje",
		[25] = "maw",		[26] = "sun",		[27] = "yun",		[28] = "lah",		[29] = "anu",
		[30] = "uwo",		[31] = "epa",		[32] = "pos",		[33] = "owi",		[34] = "ate",
		[35] = "efe",		[36] = "ibo",		[37] = "diq",		[38] = "ajo",		[39] = "ore",
		[40] = "gan",		[41] = "fuz",		[42] = "jej",		[43] = "efa",		[44] = "iho",
		[45] = "tur",		[46] = "ave",		[47] = "one",		[48] = "toz",		[49] = "mul",
		[50] = "hec",		[51] = "oja",		[52] = "suy",		[53] = "amo",		[54] = "uta",
		[55] = "buy",		[56] = "api",		[57] = "ney",		[58] = "ipo",		[59] = "vis",
		[60] = "sem",		[61] = "ave",		[62] = "uwi",		[63] = "kuw",		[64] = "oju",
		[65] = "opu",		[66] = "xed",		[67] = "xiz",		[68] = "iva",		[69] = "enu",
		[70] = "ruj",		[71] = "enu",		[72] = "ica",		[73] = "win",		[74] = "jiv",
		[75] = "piv",		[76] = "los",		[77] = "oka",		[78] = "ado",		[79] = "zos",
		[80] = "eju",		[81] = "uba",		[82] = "exo",		[83] = "yip",		[84] = "eja",
		[85] = "iso",		[86] = "oba",		[87] = "rix",		[88] = "ayo",		[89] = "yif",
		[90] = "vot",		[91] = "mul",		[92] = "yen",		[93] = "yip",		[94] = "ori",
		[95] = "azi",		[96] = "yol",		[97] = "ocu",		[98] = "ega",		[99] = "leg",
		[100] = "ace",		[101] = "ixa",		[102] = "jaw",		[103] = "ice",		[104] = "ujo",
		[105] = "xad",		[106] = "upe",		[107] = "adu",		[108] = "mis",		[109] = "dan",
		[110] = "uki",		[111] = "ebu",		[112] = "uva",		[113] = "vov",		[114] = "onu",
		[115] = "aka",		[116] = "ese",		[117] = "aja",		[118] = "not",		[119] = "ota",
		[120] = "otu",		[121] = "uxe",		[122] = "nut",		[123] = "len",		[124] = "uwu",
		[125] = "ano",		[126] = "uhe",		[127] = "raf",		[128] = "ewo",		[129] = "ile",
		[130] = "ren",		[131] = "ulu",		[132] = "xiz",		[133] = "nub",		[134] = "iko",
		[135] = "qub",		[136] = "eta",		[137] = "ama",		[138] = "xib",		[139] = "uja",
		[140] = "six",		[141] = "uzo",		[142] = "aco",		[143] = "kel",		[144] = "ife",
		[145] = "oti",		[146] = "qix",		[147] = "ije",		[148] = "for",		[149] = "uwa",
		[150] = "oju",		[151] = "oda",		[152] = "iza",		[153] = "hem",		[154] = "eda",
		[155] = "ose",		[156] = "uwu",		[157] = "usa",		[158] = "zex",		[159] = "ses",
		[160] = "iyo",		[161] = "ayu",		[162] = "yub",		[163] = "amu",		[164] = "uzu",
		[165] = "uyo",		[166] = "ces",		[167] = "oma",		[168] = "ado",		[169] = "kac",
		[170] = "uko",		[171] = "oni",		[172] = "iwo",		[173] = "cot",		[174] = "jun",
		[175] = "uce",		[176] = "aji",		[177] = "ato",		[178] = "vec",		[179] = "yip",
		[180] = "ewu",		[181] = "xaz",		[182] = "six",		[183] = "agi",		[184] = "xab",
		[185] = "xuq",		[186] = "ijo",		[187] = "ite",		[188] = "iri",		[189] = "ufu",
		[190] = "uza",		[191] = "idi",		[192] = "aza",		[193] = "oqa",		[194] = "ozu",
		[195] = "afu",		[196] = "dam",		[197] = "ata",		[198] = "egu",		[199] = "qeg",
		[200] = "oge",		[201] = "izu",		[202] = "esi",		[203] = "muh",		[204] = "fif",
		[205] = "wim",		[206] = "eka",		[207] = "upi",		[208] = "faj",		[209] = "kik",
		[210] = "eqe",		[211] = "qec",		[212] = "lik",		[213] = "pav",		[214] = "nih",
		[215] = "obo",		[216] = "ecu",		[217] = "qav",		[218] = "uri",		[219] = "iru",
		[220] = "yeq",		[221] = "quv",		[222] = "oho",		[223] = "iwe",		[224] = "xil",
		[225] = "ovo",		[226] = "xin",		[227] = "uso",		[228] = "oto",		[229] = "ozi",
		[230] = "guj",		[231] = "coz",		[232] = "exa",		[233] = "ila",		[234] = "tun",
		[235] = "gok",		[236] = "owe",		[237] = "yol",		[238] = "eno",		[239] = "diq",
		[240] = "rug",		[241] = "efu",		[242] = "uqa",		[243] = "iyi",		[244] = "ijo",
		[245] = "eco",		[246] = "veh",		[247] = "yon",		[248] = "hoz",		[249] = "epi",
		[250] = "pul",		[251] = "bes",		[252] = "ret",		[253] = "iwu",		[254] = "vif",
        [255] = "vow"
    };

    char *ptr = argv[1];
    char hex[3] = {0};
    uint8_t byte;
    uint8_t id_decode[20] = {0}; 
    uint32_t seed = 0;
    struct Planet *planet = NULL;

    for (int j = 0; j < 20; j++, ptr+=2) {
        memcpy(hex, ptr, 2);
        byte = strtol(hex, NULL, 16);
        id_decode[j] = byte;
    }

    memcpy(&seed, id_decode+8, 4);

    srand(seed);

    char continent = id_decode[13] % 127;
    char water = id_decode[11] % 127;
    char is_water = id_decode[10] % 2;
    char belt_char = id_decode[8] % 127;
    uint8_t belt = id_decode[12] % (_MAX_BELT);

    if (!isprint(water))
        water += 34;
    if (!isprint(continent))
        continent += 34;
    if (!isprint(belt_char))
        belt_char += 34;

    planet = (struct Planet *)id_decode;
    
    struct ASCIIArt art = {water, continent, belt_char, is_water, belt};
    
    puts("```");
    print_full_planet(art, rgb_to_x256(planet->color), rgb_to_x256(planet->color_belt));
    puts("```");


    printf("**Name**: %s%s%s-%02x%02x\n",
            syllabe[planet->name.sylab_a],
            syllabe[planet->name.sylab_b],
            syllabe[planet->name.sylab_c],
            planet->name.id_x,
            planet->name.id_y);

    if (is_water)
       printf("**Water char**: '%c'\n", water); 
    if (belt) {
        printf("**Belt char**: '%c'\n", art.belt_char);
        printf("**Belt color**: ");
        print_color(planet->color_belt);
        printf("\n");
    }

    printf("**Continent char**: '%c'\n", continent); 
    printf("**Color**: %02x%02x%02x\n", planet->color.r, planet->color.g, planet->color.b);
    printf("**Galaxy**:\n \tID: %02x\n\tNum: %u\n", planet->galaxy, planet->galaxy);
    printf("**Solar system**:\n \tID: %02x\n \tNum: %u\n", planet->solar_system, planet->solar_system);
    printf("**Surface**: %u.%u.%u Km2\n", planet->surface.millions, planet->surface.thousands, planet->surface.hundreds);
    printf("**Gravity**: %u.%u m/s2\n", planet->gravity.millions, planet->gravity.thousands);
    printf("**Years**: %u.%u Millions Years old\n", planet->years.millions, planet->years.thousands);

    return 0;
}
