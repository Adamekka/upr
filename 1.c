//! Nastavení prostředí, základy C

#include <stdint.h>
#include <stdio.h>

int main() {
    uint32_t input;
    puts("zadej pocet penez:");
    scanf("%d", &input);

    const uint16_t bankovky[] = {5000, 2000, 1000, 500, 200, 100};

    for (uint8_t i = 0; i < (sizeof(bankovky) / sizeof(uint16_t)); i++) {
        const uint16_t bankovka = bankovky[i];
        uint32_t pocet_bankovek = 0;

        while (bankovka <= input) {
            input -= bankovka;
            pocet_bankovek++;
        }

        printf("Bankovka %d: %dx\n", bankovka, pocet_bankovek);
    }
}
