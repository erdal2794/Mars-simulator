# Mars Kolonisi Yaşam Destek Simülatörü

Bu proje, C programlama dilinin temel kontrol yapıları kullanılarak geliştirilmiş karakter ve metin tabanlı bir hayatta kalma simülasyonudur. Oyuncu, Mars'taki araştırma istasyonunda mahsur kalan bir mühendisi yöneterek kurtarma ekibi gelene kadar **15 gün** boyunca hayatta kalmaya çalışır.

---

## Projenin Amacı ve Oynanış

Oyun başlangıcında oyuncudan bir zorluk seviyesi (**K:** Kolay, **N:** Normal, **Z:** Zor) seçmesi istenir. Seçilen seviyeye göre başlangıç kaynakları ve çevresel tehditlerin şiddeti belirlenir. Oyuncu, tek karakterli komutlar vererek istasyonun sağlık, enerji, oksijen, su, yedek parça ve modül hasarı değerlerini dengede tutmalıdır.

* **Zaman Akışı:** Geçerli her 3 turda bir yeni gün başlar. Gün başlarında oksijen seviyesi azalır ve modül hasarı kritik düzeydeyse sağlık kaybı yaşanır.
* **Kazanma Koşulu:** Sağlık ve oksijen sıfırın üzerindeyken 15. günü tamamlamak.
* **Kaybetme Koşulu:** Sağlık veya oksijen değerinin 0'a düşmesi.

---

## Komut Listesi

Komutlar büyük/küçük harf duyarsızdır:

| Komut | İşlev | Açıklama |
| :--- | :--- | :--- |
| **G** | Keşif Görevi | Enerji ve oksijen harcayarak su veya yedek parça arar. Yaralanma riski içerir. |
| **O** | Oksijen Üret | Mevcut su ve enerjiyi kullanarak oksijen üretir (2 birim su = 3 birim oksijen). |
| **T** | Tamir | Yedek parça ve enerji harcayarak modül hasarını kademeli olarak onarır. |
| **D** | Durum Raporu | Mevcut kaynakları ve kritik sistem uyarılarını gösterir (Tur harcamaz). |
| **U** | Uyu / Dinlen | Enerji ve sağlık yeniler. Oksijen 20'nin altındaysa sağlık kaybına yol açar. |
| **K** | Kum Fırtınası | Zorluk seviyesine göre belirlenen tur boyunca süren fırtınayı simüle eder. |
| **H** | Hava Kilidi | Gün sayısına göre türetilen şifreyi 3 denemede çözerek istasyonu mühürler. |
| **Q** | Çıkış | Simülasyonu sonlandırır ve oyun sonu puan raporunu ekrana yazdırır. |

---

## Kullanılan C Programlama Yapıları

* **Kontrol Yapıları:** Komut yönetimi ve zorluk seçimi için `switch-case` (*fall-through* tekniği ile) ve çok kademeli karar mekanizmaları için `if-else if-else` blokları kullanılmıştır.
* **Döngüler:** Ana oyun akışı ve şifre doğrulama mekanizması için `do-while`, kum fırtınası simülasyonu için `for` döngüsü (`break` ve `continue` ifadeleriyle birlikte) kullanılmıştır.
* **Operatörler:** Tur/gün geçişleri, çift tur hasar çarpanı ve artık su hesaplamaları için mod (`%`) operatörü; kritik durum kontrolleri için mantıksal (`&&`, `||`) operatörler kullanılmıştır.
* **Rastgelelik:** Olay ihtimalleri `srand(time(NULL))` ve `rand() % 100` fonksiyonları ile dinamik olarak hesaplanmaktadır.

---

## Kurulum ve Çalıştırma

Projeyi bilgisayarınızda derleyip çalıştırmak için bir C derleyicisi (GCC vb.) kullanabilirsiniz:

```bash
gcc marskoloni.c -o marskoloni
./marskoloni
