#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Rastgele sayi uretimi icin baslangic ayari (Zorunlu)
    srand(time(NULL));

    // Oyun Durum Degiskenleri
    int saglik = 100, enerji = 100, oksijen = 100;
    int su = 0, yedekParca = 0, modulHasari = 0;
    int kilitMuhurlu = 0; // 0: Acik, 1: Muhurlu
    int gun = 1, tur = 0;

    // Zorluk seviyesine gore degisecek esikler
    int firtinaTuru = 3;
    int firtinaHasarCarpani = 1;

    char zorluk, komut;
    int gecerliZorluk = 0;

    printf("=== MARS KOLONISI YASAM DESTEK SIMULATORU ===\n");

    // 1. ZORLUK SECIMI (Gecersiz giriste tekrar sorar)
    while (gecerliZorluk == 0) {
        printf("Zorluk Seviyesi Secin (K: Kolay, N: Normal, Z: Zor): ");
        scanf(" %c", &zorluk);

        switch (zorluk) {
            case 'k':
            case 'K':
                su = 25; yedekParca = 5; firtinaTuru = 3; firtinaHasarCarpani = 1;
                gecerliZorluk = 1;
                printf("Kolay mod secildi.\n");
                break;
            case 'n':
            case 'N':
                su = 15; yedekParca = 3; firtinaTuru = 5; firtinaHasarCarpani = 2;
                gecerliZorluk = 1;
                printf("Normal mod secildi.\n");
                break;
            case 'z':
            case 'Z':
                su = 8; yedekParca = 1; firtinaTuru = 7; firtinaHasarCarpani = 3;
                gecerliZorluk = 1;
                printf("Zor mod secildi.\n");
                break;
            default:
                printf("HATA: Gecersiz zorluk seviyesi! Tekrar deneyin.\n");
        }
    }

    // 2. ANA OYUN DONGUSU (DO-WHILE Zorunlu)
    do {
        printf("\n--------------------------------------------------\n");
        printf("GUN: %d | TUR: %d | Saglik: %d | Enerji: %d | Oksijen: %d\n", gun, tur, saglik, enerji, oksijen);
        printf("Komutlar: [G]esif [O]ksijen [T]amir [D]urum [U]yu [K]um Firtinasi [H]ava Kilidi [Q]Cikis\n");
        printf("Komutunuz: ");
        scanf(" %c", &komut);

        int turGecti = 0; // D ve gecersiz komutlarda tur artmamasi icin kontrol

        // KOMUT SISTEMI (SWITCH-CASE ve Fall-Through Zorunlu)
        switch (komut) {
            case 'g':
            case 'G': { // KESIF GOREVI
                if (enerji < 15 || oksijen < 10) {
                    printf("UYARI: Kesif icin yeterli enerji (15) veya oksijen (10) yok!\n");
                } else {
                    enerji -= 15;
                    oksijen -= 10;
                    turGecti = 1;

                    // Mantiksal operatorle esik belirleme
                    int esik = (enerji > 40 && oksijen > 30) ? 60 : 35;
                    int zar = rand() % 100;

                    if (zar < esik) {
                        printf("Zar: %d / Esik: %d -> BASARILI! ", zar, esik);
                        // Su mu yedek parca mi bulunacak?
                        if (zar % 2 == 0) {
                            su += 6;
                            printf("6 birim SU buldunuz!\n");
                        } else {
                            yedekParca += 2;
                            printf("2 adet YEDEK PARCA buldunuz!\n");
                        }
                    } else {
                        printf("Zar: %d / Esik: %d -> BASARISIZ! Eli bos dondunuz.\n", zar, esik);
                    }

                    // Yaralanma kontrolu (Mantiksal VEYA)
                    int kazaZari = rand() % 100;
                    if (kazaZari < 10 || modulHasari > 70) {
                        saglik -= 15;
                        printf("DIKKAT! Kesif sirasinda yaralandiniz (-15 Saglik). (Zar: %d / Esik: 10)\n", kazaZari);
                    }
                }
                break;
            }

            case 'o':
            case 'O': { // OKSIJEN URETIMI
                if (enerji < 10) {
                    printf("UYARI: Oksijen uretmek icin en az 10 enerji gerekli!\n");
                } else if (su < 2) {
                    printf("UYARI: Oksijen uretmek icin en az 2 birim su gerekli!\n");
                } else {
                    int kullanilacakSu;
                    printf("Mevcut Su: %d. Kac birim su kullanacaksiniz?: ", su);
                    scanf("%d", &kullanilacakSu);

                    if (kullanilacakSu > su || kullanilacakSu < 2) {
                        printf("HATA: Gecersiz su miktari!\n");
                    } else {
                        enerji -= 10;
                        turGecti = 1;
                        // Mod operatoru ile cift/tek kontrolu (2 su = 3 oksijen)
                        int artikSu = kullanilacakSu % 2;
                        int islenenSu = kullanilacakSu - artikSu;
                        int uretilenOksijen = (islenenSu / 2) * 3;

                        su -= islenenSu;
                        oksijen += uretilenOksijen;
                        printf("%d birim su islendi, +%d oksijen uretildi. (Depoda kalan artik su: %d)\n", islenenSu, uretilenOksijen, artikSu);
                    }
                }
                break;
            }

            case 't':
            case 'T': { // TAMIR
                if (modulHasari == 0) {
                    printf("Modullerde hic hasar yok, tamire gerek yok.\n");
                } else if (enerji < 10) {
                    printf("UYARI: Tamir icin en az 10 enerji gerekli!\n");
                } else {
                    // IF - ELSE IF - ELSE zinciri ile 3 hasar kademesi
                    if (modulHasari <= 30) { // Hafif hasar
                        if (yedekParca >= 1) {
                            yedekParca -= 1; enerji -= 10; modulHasari -= 15; turGecti = 1;
                            printf("Hafif hasar onarildi (-1 Parca, -15 Hasar).\n");
                        } else printf("Yetersiz yedek parca (1 gerekli)!\n");
                    } else if (modulHasari <= 70) { // Orta hasar
                        if (yedekParca >= 2) {
                            yedekParca -= 2; enerji -= 10; modulHasari -= 30; turGecti = 1;
                            printf("Orta hasar onarildi (-2 Parca, -30 Hasar).\n");
                        } else printf("Yetersiz yedek parca (2 gerekli)!\n");
                    } else { // Agir hasar (71-100)
                        if (yedekParca >= 3) {
                            yedekParca -= 3; enerji -= 10; modulHasari -= 45; turGecti = 1;
                            printf("Agir hasar onarildi (-3 Parca, -45 Hasar).\n");
                        } else printf("Yetersiz yedek parca (3 gerekli)!\n");
                    }
                }
                break;
            }

            case 'd':
            case 'D': { // DURUM RAPORU (Tur harcamaz)
                printf("\n--- DETAYLI DURUM RAPORU ---\n");
                printf("Saglik: %d | Enerji: %d | Oksijen: %d\n", saglik, enerji, oksijen);
                printf("Su: %d | Yedek Parca: %d | Modul Hasari: %d | Hava Kilidi: %s\n",
                       su, yedekParca, modulHasari, kilitMuhurlu ? "MUHURLU" : "ACIK");

                if (oksijen < 25) printf("[KRITIK UYARI] Oksijen seviyesi cok dusuk!\n");
                if (saglik < 30) printf("[KRITIK UYARI] Saglik durumu kritik!\n");
                if (modulHasari > 70) printf("[KRITIK UYARI] Modul hasari cok yuksek!\n");

                // Tum degerler guvenliyse tek kosulla mesaj
                if (oksijen >= 25 && saglik >= 30 && modulHasari <= 70 && enerji >= 20) {
                    printf(">> TUM SISTEMLER NORMAL <<\n");
                }
                break;
            }

            case 'u':
            case 'U': { // UYU / DINLEN
                turGecti = 1;
                enerji += 25;
                oksijen -= 10;
                // Ic ice IF ile oksijen kontrolu
                if (oksijen < 20) {
                    saglik -= 10;
                    printf("Uyudunuz (+25 Enerji), ANCAK oksijen 20'nin altinda oldugu icin saglik kaybettiniz (-10 Saglik)!\n");
                } else {
                    saglik += 15;
                    printf("Guzel bir uyku cektiniz (+25 Enerji, +15 Saglik, -10 Oksijen).\n");
                }
                break;
            }

            case 'k':
            case 'K': { // KUM FIRTINASI (FOR Dongusu Zorunlu)
                turGecti = 1;
                int hasarAlinanTur = 0, topSaglikKaybi = 0, topOksijenKaybi = 0;
                printf("\n!!! KUM FIRTINASI BASLADI (%d Tur Surecek) !!!\n", firtinaTuru);

                for (int i = 1; i <= firtinaTuru; i++) {
                    int zar = rand() % 100;
                    int esik = 50;
                    // Cift turlarda ruzgar siddetlenir (Mod operatoru)
                    int carpan = (i % 2 == 0) ? 2 : 1;

                    printf("Firtina Turu %d -> Zar: %d / Esik: %d -> ", i, zar, esik);

                    if (zar < esik) {
                        hasarAlinanTur++;
                        int olay = zar % 3; // 0: Can, 1: Oksijen, 2: Hasar

                        if (olay == 1 && kilitMuhurlu == 1) {
                            printf("Hava kilidi muhurlu oldugu icin oksijen sizintisi engellendi!\n");
                            continue; // Oksijen sizintisini atla (Zorunlu)
                        }

                        if (olay == 0) {
                            int kayip = 5 * carpan * firtinaHasarCarpani;
                            saglik -= kayip; topSaglikKaybi += kayip;
                            printf("BASARISIZ (Enkaz carpmasi: -%d Saglik)\n", kayip);
                        } else if (olay == 1) {
                            int kayip = 6 * carpan * firtinaHasarCarpani;
                            oksijen -= kayip; topOksijenKaybi += kayip;
                            printf("BASARISIZ (Oksijen sizintisi: -%d Oksijen)\n", kayip);
                        } else {
                            int hasar = 8 * carpan;
                            modulHasari += hasar;
                            printf("BASARISIZ (Modul hasari aldi: +%d Hasar)\n", hasar);
                        }
                    } else {
                        printf("BASARILI (Firtinadan korundunuz)\n");
                    }

                    // Can veya oksijen 0'a duserse donguyu kir (Zorunlu)
                    if (saglik <= 0 || oksijen <= 0) {
                        printf("KRITIK DURUM! Firtina sirasinda yasam destegi coktu!\n");
                        break;
                    }
                }
                printf("--- Firtina Ozeti: %d turda hasar alindi | Kayip Saglik: %d | Kayip Oksijen: %d ---\n",
                       hasarAlinanTur, topSaglikKaybi, topOksijenKaybi);
                break;
            }

            case 'h':
            case 'H': { // HAVA KILIDI SIFRESI (DO-WHILE Zorunlu)
                if (kilitMuhurlu == 1) {
                    printf("Hava kilidi zaten muhurlu!\n");
                } else if (enerji < 15) {
                    printf("UYARI: Sifre denemeleri icin yeterli enerjiniz yok!\n");
                } else {
                    turGecti = 1;
                    // Gun sayisindan turetilen dinamik sifre
                    char dogruSifre = 'A' + (gun % 26);
                    char tahmin;
                    int denemeHakki = 3;
                    int sifreCozuldu = 0;

                    printf("\n[HAVA KILIDI] Ipucu: Sifre, alfabede 'A' harfinden bugunun gun sayisi (%d) kadar ilerideki harftir.\n", gun);

                    // Tek mantiksal ifadeli DO-WHILE kosulu (Zorunlu)
                    do {
                        printf("Kalan Hak: %d | Tek harf sifre girin: ", denemeHakki);
                        scanf(" %c", &tahmin);

                        // Kucuk harf girildiyse buyuk harfe cevir (ASCII farki: 32)
                        if (tahmin >= 'a' && tahmin <= 'z') {
                            tahmin = tahmin - 32;
                        }

                        if (tahmin == dogruSifre) {
                            sifreCozuldu = 1;
                            kilitMuhurlu = 1;
                            printf("BASARILI! Hava kilidi muhurlendi.\n");
                        } else {
                            denemeHakki--;
                            enerji -= 5;
                            if (tahmin < dogruSifre) {
                                printf("YANLIS! Ipucu: Alfabede ILERI git (-5 Enerji).\n");
                            } else {
                                printf("YANLIS! Ipucu: Alfabede GERI git (-5 Enerji).\n");
                            }
                        }
                    } while (sifreCozuldu == 0 && denemeHakki > 0);

                    if (sifreCozuldu == 0) {
                        modulHasari += 20;
                        printf("BASARISIZ! 3 hakkiniz bitti. Sistem kilitlendi (+20 Modul Hasari).\n");
                    }
                }
                break;
            }

            case 'q':
            case 'Q':
                printf("Simulasyondan cikis yapiliyor...\n");
                break;

            default:
                printf("HATA: Tanimsiz komut! Tur harcanmadi.\n");
        }

        // SINIRLANDIRMA KONTROLU (0-100 arasi tutma - Zorunlu)
        if (saglik > 100) saglik = 100; if (saglik < 0) saglik = 0;
        if (enerji > 100) enerji = 100; if (enerji < 0) enerji = 0;
        if (oksijen > 100) oksijen = 100; if (oksijen < 0) oksijen = 0;
        if (modulHasari > 100) modulHasari = 100; if (modulHasari < 0) modulHasari = 0;

        // ZAMAN AKISI VE GUN DEGISTIRME
        if (turGecti == 1) {
            tur++;
            if (tur % 3 == 0) {
                gun++;
                oksijen -= 8;
                printf("\n*** YENI GUN BASLADI (Gun %d)! Gunluk oksijen tuketimi: -8 ***\n", gun);
                if (modulHasari > 70) {
                    saglik -= 5;
                    printf("*** UYARI: Modul hasari 70 uzerinde oldugu icin -5 Saglik kaybettiniz! ***\n");
                }
                // Gun basi dususleri sonrasi tekrar sinirlandirma
                if (saglik < 0) saglik = 0;
                if (oksijen < 0) oksijen = 0;
            }
        }

    } while (komut != 'q' && komut != 'Q' && saglik > 0 && oksijen > 0 && gun <= 15);

    // 3. OYUN SONU RAPORU VE PUANLAMA
    printf("\n==================================================\n");
    if (gun > 15 && saglik > 0 && oksijen > 0) {
        printf("TEBRIKLER! 15 gunu tamamladiniz ve kurtarma ekibi ulasti!\n");
    } else if (saglik <= 0 || oksijen <= 0) {
        printf("OYUN BITTI! Yasam destek sistemleri tukendi ve hayatini kaybettin.\n");
    } else {
        printf("SIMULASYON SONLANDIRILDI.\n");
    }

    int puan = (gun * 100) + saglik + oksijen - modulHasari;
    printf("Hayatta Kalinan Gun: %d | Toplam Tur: %d\n", (gun > 15 ? 15 : gun), tur);
    printf("Son Durum -> Saglik: %d | Enerji: %d | Oksijen: %d | Modul Hasari: %d\n", saglik, enerji, oksijen, modulHasari);
    printf("TOPLAM PUANINIZ: %d\n", puan);
    printf("==================================================\n");

    return 0;
}
