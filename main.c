#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Rastgele sayi ureticisini baslatma
    srand(time(NULL));

    // Durum degiskenleri
    int saglik = 100;
    int enerji = 100;
    int oksijen = 100;
    int su = 0;
    int yedekParca = 0;
    int modulHasari = 0;
    int kilitMuhurlu = 0; // 0: Acik, 1: Muhurlu

    int gun = 1;
    int tur = 0;
    int puan = 0;

    // Zorluk seviyesine bagli parametreler
    char zorluk;
    int zorlukGecerli = 0;
    int kesifEsikBonus = 0;
    int firtinaTurSayisi = 5;
    int firtinaHasarCarpani = 1;

    printf("==================================================\n");
    printf("   MARS KOLONISI YASAM DESTEK SIMULATORU (v1.0)   \n");
    printf("==================================================\n");
    printf("Kurtarma ekibi gelene kadar 15 gun hayatta kal!\n\n");

    // 1. ZORLUK SEVIYESI SECIMI (DO-WHILE ve SWITCH-CASE)
    do {
        printf("Zorluk Seviyesi Secin [K: Kolay, N: Normal, Z: Zor]: ");
        scanf(" %c", &zorluk);

        switch (zorluk) {
            case 'k':
            case 'K':
                saglik = 100;
                enerji = 100;
                oksijen = 100;
                su = 25;
                yedekParca = 6;
                modulHasari = 10;
                kesifEsikBonus = 15;
                firtinaTurSayisi = 3;
                firtinaHasarCarpani = 1;
                zorlukGecerli = 1;
                printf("\n[BILGI] KOLAY zorluk secildi. Baslangic kaynaklari yuksek.\n");
                break;

            case 'n':
            case 'N':
                saglik = 90;
                enerji = 90;
                oksijen = 85;
                su = 15;
                yedekParca = 4;
                modulHasari = 20;
                kesifEsikBonus = 0;
                firtinaTurSayisi = 5;
                firtinaHasarCarpani = 1;
                zorlukGecerli = 1;
                printf("\n[BILGI] NORMAL zorluk secildi. Standart Mars kosullari.\n");
                break;

            case 'z':
            case 'Z':
                saglik = 80;
                enerji = 80;
                oksijen = 75;
                su = 8;
                yedekParca = 2;
                modulHasari = 35;
                kesifEsikBonus = -10;
                firtinaTurSayisi = 7;
                firtinaHasarCarpani = 2;
                zorlukGecerli = 1;
                printf("\n[BILGI] ZOR zorluk secildi. Kaynaklar kisitli, firtinalar olumcul!\n");
                break;

            default:
                printf("[HATA] Gecersiz zorluk seviyesi! Lutfen K, N veya Z giriniz.\n");
                zorlukGecerli = 0;
                break;
        }
    } while (zorlukGecerli == 0);

    // 2. ANA KOMUT DINLEME DONGUSU (DO-WHILE)
    char komut;
    int oyunDevam = 1;

    do {
        printf("\n--------------------------------------------------\n");
        printf("GUN: %d / 15 | TUR: %d | Saglik: %d | Enerji: %d | Oksijen: %d\n",
               gun, tur, saglik, enerji, oksijen);
        printf("Su: %d | Yedek Parca: %d | Modul Hasari: %d | Hava Kilidi: %s\n",
               su, yedekParca, modulHasari, (kilitMuhurlu == 1) ? "MUHURLU" : "ACIK");
        printf("--------------------------------------------------\n");
        printf("Komutlar: [G]Kesif [O]Oksijen [T]Tamir [D]Durum [U]Uyu [K]Kum Firtinasi [H]Hava Kilidi [Q]Cikis\n");
        printf("Komutunuz: ");
        scanf(" %c", &komut);

        int turGecerli = 0; // Sadece gecerli ve zaman harcayan komutlarda 1 yapilacak

        switch (komut) {
            // --------------------------------------------------
            // G: KESIF GOREVI
            // --------------------------------------------------
            case 'g':
            case 'G': {
                if (enerji < 15) {
                    printf("[UYARI] Kesif icin yeterli enerjiniz yok! (Gereken: 15, Mevcut: %d)\n", enerji);
                    break;
                }

                enerji -= 15;
                oksijen -= 10;

                int zar = rand() % 100;
                int suEsik = ((enerji > 40 && oksijen > 30) ? 55 : 35) + kesifEsikBonus;
                int parcaEsik = suEsik + 25;

                printf("[KESIF] Zar: %d / Su Esigi: %d / Parca Esigi: %d -> ", zar, suEsik, parcaEsik);

                if (zar < suEsik) {
                    int bulunanSu = (rand() % 6) + 5; // 5-10 arasi su
                    su += bulunanSu;
                    printf("BASARILI! Yeralti buzu eritildi (+%d Su).\n", bulunanSu);
                } else if (zar < parcaEsik) {
                    int bulunanParca = (rand() % 2) + 1; // 1-2 arasi yedek parca
                    yedekParca += bulunanParca;
                    printf("BASARILI! Eski uydu enkazindan parca sokuldu (+%d Yedek Parca).\n", bulunanParca);
                } else {
                    printf("BASARISIZ! Kayalik arazide eli bos donuldu.\n");
                }

                // Yaralanma kontrolu
                int yaraZar = rand() % 100;
                printf("[RISK KONTROLU] Zar: %d / Esik: 10 (veya Hasar > 70) -> ", yaraZar);
                if (yaraZar < 10 || modulHasari > 70) {
                    saglik -= 12;
                    printf("YARALANDINIZ! (-12 Saglik)\n");
                } else {
                    printf("GUVENLI.\n");
                }

                turGecerli = 1;
                break;
            }

            // --------------------------------------------------
            // O: OKSIJEN URET
            // --------------------------------------------------
            case 'o':
            case 'O': {
                if (enerji < 10) {
                    printf("[UYARI] Oksijen jeneratoru icin yeterli enerjiniz yok! (Gereken: 10, Mevcut: %d)\n", enerji);
                    break;
                }
                if (su < 2) {
                    printf("[UYARI] Oksijen uretimi icin en az 2 birim su gereklidir! (Mevcut: %d)\n", su);
                    break;
                }

                int harcanacakSu;
                printf("Kullanilacak su miktarini girin (Mevcut Su: %d): ", su);
                scanf("%d", &harcanacakSu);

                if (harcanacakSu <= 0 || harcanacakSu > su) {
                    printf("[HATA] Gecersiz miktar! Mevcut sudan fazla veya 0/negatif giremezsiniz.\n");
                    break;
                }
                if (harcanacakSu < 2) {
                    printf("[UYARI] En az 2 birim su girilmelidir (2 birim su = 3 oksijen).\n");
                    break;
                }

                // Mod (%) operatoru ile tek/cift kontrolu
                int artikSu = harcanacakSu % 2;
                int islenenSu = harcanacakSu - artikSu;
                int uretilenOksijen = (islenenSu / 2) * 3;

                su -= islenenSu;
                enerji -= 10;
                oksijen += uretilenOksijen;

                printf("[OKSIJEN URETIMI] %d birim su islendi -> +%d Oksijen uretildi.\n", islenenSu, uretilenOksijen);
                if (artikSu == 1) {
                    printf("[BILGI] Girilen miktar tek sayi oldugu icin 1 birim su artik olarak depoda birakildi.\n");
                }

                turGecerli = 1;
                break;
            }

            // --------------------------------------------------
            // T: TAMIR
            // --------------------------------------------------
            case 't':
            case 'T': {
                if (enerji < 10) {
                    printf("[UYARI] Tamir islemi icin yeterli enerjiniz yok! (Gereken: 10, Mevcut: %d)\n", enerji);
                    break;
                }
                if (yedekParca <= 0) {
                    printf("[UYARI] Tamir yapilamaz! Hic yedek parcaniz kalmadi.\n");
                    break;
                }
                if (modulHasari == 0) {
                    printf("[BILGI] Istasyon modullerinde hasar bulunmuyor. Tamire gerek yok.\n");
                    break;
                }

                // Hasar seviyesine gore IF - ELSE IF - ELSE zinciri
                if (modulHasari <= 30) {
                    // Hafif Hasar
                    yedekParca -= 1;
                    enerji -= 10;
                    modulHasari -= 15;
                    printf("[TAMIR - HAFIF HASAR] 1 yedek parca kullanildi, 15 birim hasar onarildi.\n");
                } else if (modulHasari <= 70) {
                    // Orta Hasar
                    if (yedekParca < 2) {
                        printf("[UYARI] Orta seviye hasar icin 2 yedek parca gerekiyor! (Mevcut: %d)\n", yedekParca);
                        break;
                    }
                    yedekParca -= 2;
                    enerji -= 10;
                    modulHasari -= 30;
                    printf("[TAMIR - ORTA HASAR] 2 yedek parca kullanildi, 30 birim hasar onarildi.\n");
                } else {
                    // Agir Hasar (71 - 100)
                    if (yedekParca < 3) {
                        printf("[UYARI] Agir seviye hasar icin 3 yedek parca gerekiyor! (Mevcut: %d)\n", yedekParca);
                        break;
                    }
                    yedekParca -= 3;
                    enerji -= 10;
                    modulHasari -= 45;
                    printf("[TAMIR - AGIR HASAR] 3 yedek parca kullanildi, 45 birim hasar onarildi.\n");
                }

                turGecerli = 1;
                break;
            }

            // --------------------------------------------------
            // D: DURUM RAPORU (Tur harcamaz)
            // --------------------------------------------------
            case 'd':
            case 'D': {
                printf("\n================ DETAYLI DURUM RAPORU ================\n");
                printf("Gun: %d / 15 | Toplam Gecerli Tur: %d\n", gun, tur);
                printf("Saglik       : %d / 100\n", saglik);
                printf("Enerji       : %d / 100\n", enerji);
                printf("Oksijen      : %d / 100\n", oksijen);
                printf("Su Stogu     : %d birim\n", su);
                printf("Yedek Parca  : %d adet\n", yedekParca);
                printf("Modul Hasari : %d / 100\n", modulHasari);
                printf("Hava Kilidi  : %s\n", (kilitMuhurlu == 1) ? "MUHURLU (Guvenli)" : "ACIK (Riskli)");
                printf("------------------------------------------------------\n");

                // Kritik uyarilar
                if (oksijen < 25) {
                    printf("[KRITIK UYARI] Oksijen seviyesi tehlikeli derecede dusuk! (< 25)\n");
                }
                if (saglik < 30) {
                    printf("[KRITIK UYARI] Saglik durumunuz kritik! (< 30)\n");
                }
                if (modulHasari > 70) {
                    printf("[KRITIK UYARI] Modul hasari cok yuksek! Istasyon cokebilir! (> 70)\n");
                }
                if (enerji < 20) {
                    printf("[KRITIK UYARI] Enerji tukenmek uzere! (< 20)\n");
                }

                // Tek bir && kosulu ile tum sistemlerin normalligi
                if (saglik >= 30 && enerji >= 20 && oksijen >= 25 && modulHasari <= 70) {
                    printf("[SISTEM DURUMU] TUM SISTEMLER NORMAL\n");
                }
                printf("======================================================\n");

                turGecerli = 0; // Durum raporu tur harcamaz
                break;
            }

            // --------------------------------------------------
            // U: UYU / DINLEN
            // --------------------------------------------------
            case 'u':
            case 'U': {
                enerji += 25;
                oksijen -= 10;

                // Ic ice IF ile oksijen kontrolu
                if (oksijen < 20) {
                    saglik -= 10;
                    printf("[DINLENME] Enerji artti (+25) ancak dusuk oksijen (<20) nedeniyle bogulma tehlikesi gecirdiniz (-10 Saglik)!\n");
                } else {
                    if (saglik < 100) {
                        saglik += 12;
                        printf("[DINLENME] Verimli bir uyku cektiniz (+25 Enerji, +12 Saglik, -10 Oksijen).\n");
                    } else {
                        printf("[DINLENME] Dinlendiniz (+25 Enerji, -10 Oksijen). Sagliginiz zaten tam.\n");
                    }
                }

                turGecerli = 1;
                break;
            }

            // --------------------------------------------------
            // K: KUM FIRTINASI SIMULASYONU (FOR DONGUSU)
            // --------------------------------------------------
            case 'k':
            case 'K': {
                if (enerji < 10) {
                    printf("[UYARI] Firtina onlemleri almak icin yeterli enerjiniz yok! (Gereken: 10, Mevcut: %d)\n", enerji);
                    break;
                }

                enerji -= 10;
                printf("\n>>> KUM FIRTINASI BASLADI! (Sure: %d Saat/Tur) <<<\n", firtinaTurSayisi);

                int hasarAlinanTur = 0;
                int topSaglikKaybi = 0;
                int topOksijenKaybi = 0;

                for (int i = 1; i <= firtinaTurSayisi; i++) {
                    int zar = rand() % 100;
                    int esik = 65; // 65 alti olumsuz olay
                    int siddetCarpani = (i % 2 == 0) ? 2 : 1; // Cift turlarda ruzgar 2 kat siddetli

                    printf("  [Firtina Saati %d/%d] Zar: %d / Esik: %d ", i, firtinaTurSayisi, zar, esik);
                    if (siddetCarpani == 2) {
                        printf("(CIFT TUR - 2X SIDDET!) ");
                    }

                    if (zar >= esik) {
                        printf("-> Firtinadan zararsiz korundunuz.\n");
                    } else if (zar < 25) {
                        // Oksijen sizintisi olayi
                        if (kilitMuhurlu == 1) {
                            printf("-> Hava kilidi MUHURLU oldugu icin oksijen sizintisi engellendi! (Atlandi)\n");
                            continue; // Hava kilidi muhurluyse bu turu atla
                        }
                        int oxKayip = 6 * siddetCarpani * firtinaHasarCarpani;
                        oksijen -= oxKayip;
                        topOksijenKaybi += oxKayip;
                        hasarAlinanTur++;
                        printf("-> Oksijen sizintisi olustu! (-%d Oksijen)\n", oxKayip);
                    } else if (zar < 45) {
                        // Can kaybi olayi
                        int canKayip = 5 * siddetCarpani * firtinaHasarCarpani;
                        saglik -= canKayip;
                        topSaglikKaybi += canKayip;
                        hasarAlinanTur++;
                        printf("-> Ucan enkaz parcasi carpti! (-%d Saglik)\n", canKayip);
                    } else {
                        // Modul hasari olayi
                        int modHasar = 7 * siddetCarpani * firtinaHasarCarpani;
                        modulHasari += modHasar;
                        hasarAlinanTur++;
                        printf("-> Dis paneller hasar aldi! (+%d Modul Hasari)\n", modHasar);
                    }

                    // Saglik veya oksijen 0'a duserse donguyu derhal sonlandir
                    if (saglik <= 0 || oksijen <= 0) {
                        printf("  [KRITIK] Yasamsal degerler tukendi! Firtina simulasyonu kesiliyor...\n");
                        break;
                    }
                }

                printf(">>> FIRTINA OZETI: Hasar Alinan Tur: %d | Toplam Saglik Kaybi: %d | Toplam Oksijen Kaybi: %d <<<\n",
                       hasarAlinanTur, topSaglikKaybi, topOksijenKaybi);

                turGecerli = 1;
                break;
            }

            // --------------------------------------------------
            // H: HAVA KILIDI SIFRESI (DO-WHILE DOGRULAMA DONGUSU)
            // --------------------------------------------------
            case 'h':
            case 'H': {
                if (kilitMuhurlu == 1) {
                    printf("[BILGI] Hava kilidi zaten muhurlu durumda!\n");
                    break;
                }
                if (enerji < 15) {
                    printf("[UYARI] Sifre panelini calistirmak icin en az 15 enerji gerekir! (Mevcut: %d)\n", enerji);
                    break;
                }

                // Gun sayisindan aritmetik olarak turetilen sifre
                char dogruSifre = 'A' + (gun % 26);
                char tahmin;
                int deneme = 0;
                int maxDeneme = 3;
                int basarili = 0;

                printf("\n[HAVA KILIDI GUVENLIK SISTEMI]\n");
                printf("Ipucu: Sifre, Ingiliz alfabesinde 'A' harfinden bugunun gun sayisi (%d) kadar ilerideki harftir (mod 26).\n", gun);

                do {
                    printf("Sifreyi girin (Kalan Hak: %d): ", maxDeneme - deneme);
                    scanf(" %c", &tahmin);

                    // Kucuk harf girisini buyuk harfe cevirme (ASCII aritmetigi)
                    if (tahmin >= 'a' && tahmin <= 'z') {
                        tahmin = tahmin - ('a' - 'A');
                    }

                    deneme++;

                    if (tahmin == dogruSifre) {
                        basarili = 1;
                        kilitMuhurlu = 1;
                        printf("[BASARILI] Dogru sifre! Hava kilidi basariyla MUHURLENDI.\n");
                    } else {
                        enerji -= 5;
                        printf("[HATALI SIFRE] Enerji 5 azaldi (Mevcut Enerji: %d). ", (enerji < 0) ? 0 : enerji);
                        if (tahmin < dogruSifre) {
                            printf("Ipucu: Alfabede ILERI git!\n");
                        } else {
                            printf("Ipucu: Alfabede GERI git!\n");
                        }
                    }
                } while (basarili == 0 && deneme < maxDeneme);

                if (basarili == 0) {
                    modulHasari += 20;
                    printf("[ALARM] 3 deneme hakki doldu! Guvenlik kilitlendi ve sistem zorlandi (+20 Modul Hasari).\n");
                }

                turGecerli = 1;
                break;
            }

            // --------------------------------------------------
            // Q: CIKIS
            // --------------------------------------------------
            case 'q':
            case 'Q': {
                printf("\n[CIKIS] Gorev oyuncu tarafindan sonlandirildi.\n");
                oyunDevam = 0;
                break;
            }

            // --------------------------------------------------
            // TANIMSIZ KOMUT
            // --------------------------------------------------
            default: {
                printf("[HATA] Tanimsiz komut '%c'! Lutfen gecerli bir komut girin. (Tur harcanmadi)\n", komut);
                turGecerli = 0;
                break;
            }
        }

        // 3. SINIRLANDIRMA KONTROLU (0 - 100 ARALIGI)
        if (saglik < 0) saglik = 0;
        if (saglik > 100) saglik = 100;

        if (enerji < 0) enerji = 0;
        if (enerji > 100) enerji = 100;

        if (oksijen < 0) oksijen = 0;
        if (oksijen > 100) oksijen = 100;

        if (modulHasari < 0) modulHasari = 0;
        if (modulHasari > 100) modulHasari = 100;

        // 4. ZAMAN AKISI VE GUN GECISI (Gecerli komutlarda)
        if (oyunDevam == 1 && turGecerli == 1) {
            tur++;

            // Her 3 turda bir yeni gun baslar
            if (tur % 3 == 0) {
                gun++;
                oksijen -= 8;
                printf("\n*** YENI GUN BASLADI (Gun %d) *** Gece boyunca 8 birim oksijen tuketildi.\n", gun);

                if (modulHasari > 70) {
                    saglik -= 5;
                    printf("*** [KRITIK HASAR ETKISI] Modul hasari 70'in uzerinde oldugu icin 5 saglik kaybettiniz! ***\n");
                }


 // Gun basi degisimleri sonrasi tekrar sinir kontrolu
                if (oksijen < 0) oksijen = 0;
                if (saglik < 0) saglik = 0;
            }
        }

        // 5. OYUN SONU KOSULLARI KONTROLU
        if (oyunDevam == 1) {
            if (saglik <= 0 || oksijen <= 0) {
                printf("\n==================================================\n");
                printf("       OYUN BITTI - HAYATTA KALAMADINIZ!          \n");
                if (saglik <= 0) printf("Sebep: Sagliginiz sifira dustu.\n");
                if (oksijen <= 0) printf("Sebep: Oksijeniniz tukendi.\n");
                printf("==================================================\n");
                oyunDevam = 0;
            } else if (gun > 15) {
                // 15. gun tamamlandiginda (16. gune gecildiginde) kurtarma ekibi gelir
                gun = 15; // Raporda 15 gun hayatta kalindi gorunmesi icin
                printf("\n==================================================\n");
                printf("   TEBRIKLER! 15 GUNU TAMAMLADINIZ VE KURTULDUNUZ!\n");
                printf("==================================================\n");
                oyunDevam = 0;
            }
        }

    } while (oyunDevam == 1);

    // 6. SONUC RAPORU VE PUAN HESAPLAMA
    puan = (gun * 100) + saglik + oksijen - modulHasari;
    if (puan < 0) puan = 0;

    printf("\n================ SIMULASYON SONUC RAPORU ================\n");
    printf("Hayatta Kalinan Gun : %d\n", gun);
    printf("Toplam Gecerli Tur  : %d\n", tur);
    printf("Son Saglik          : %d\n", saglik);
    printf("Son Enerji          : %d\n", enerji);
    printf("Son Oksijen         : %d\n", oksijen);
    printf("Kalan Su / Parca    : %d Su / %d Yedek Parca\n", su, yedekParca);
    printf("Son Modul Hasari    : %d\n", modulHasari);
    printf("---------------------------------------------------------\n");
    printf("TOPLAM PUAN         : %d\n", puan);
    printf("=========================================================\n");

    return 0;
}