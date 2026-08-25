*This project has been created as part of the 42 curriculum by muhaoz.*

<div align="center">

# 📄 get_next_line

![42](https://img.shields.io/badge/42-Kocaeli-000000?style=flat)
![C](https://img.shields.io/badge/language-C-blue.svg)
![Norm](https://img.shields.io/badge/norme-42-lightgrey)
![Status](https://img.shields.io/badge/mandatory-completed-brightgreen)

**Bir dosya betimleyicisinden (file descriptor) satır satır okuma yapan, `static` değişken kullanan bir C fonksiyonu.**

</div>

---

## 📑 İçindekiler

1. [Açıklama](#-açıklama)
2. [Özellikler](#-özellikler)
3. [Proje Yapısı](#-proje-yapısı)
4. [Kurulum ve Kullanım Talimatları](#️-kurulum-ve-kullanım-talimatları)
5. [Algoritma: Detaylı Açıklama ve Gerekçelendirme](#-algoritma-detaylı-açıklama-ve-gerekçelendirme)
6. [Fonksiyon Referansı](#-fonksiyon-referansı)
7. [Kısıtlamalar ve Norm Uyumu](#-kısıtlamalar-ve-norm-uyumu)
8. [Undefined Behavior ve Bilinen Sınırlamalar](#-undefined-behavior-ve-bilinen-sınırlamalar)
9. [Test Önerileri](#-test-önerileri)
10. [Kaynaklar](#-kaynaklar)
11. [Yazar](#-yazar)

---

## 📝 Açıklama

**get_next_line**, 42 müfredatının temel projelerinden biridir ve amacı, bir dosya betimleyicisinden (file descriptor) **tek seferde bir satır** okuyup döndüren bir fonksiyon yazmaktır. Fonksiyon, ardışık çağrılar arasında okuma durumunu (state) hatırlamak zorundadır; bu da projeyi C dilindeki **`static` değişkenler** kavramını uygulamalı olarak öğrenmek için ideal bir alıştırma hâline getirir.

Fonksiyon, düşük seviye `read()` sistem çağrısını kullanarak dosyadan `BUFFER_SIZE` boyutunda parçalar (chunk) okur, bu parçaları satır sonu (`\n`) karakteri bulunana kadar bir tampon bellekte (stash) biriktirir ve her çağrıda yalnızca bir satırı çağırana döndürür. Kalan veri bir sonraki çağrı için `static` bir değişkende saklanır.

Bu depo, projenin **zorunlu (mandatory) kısmını** içerir:

- `get_next_line.c`
- `get_next_line.h`
- `get_next_line_utils.c`

> ℹ️ Bonus kısım (birden fazla file descriptor'ı tek bir `static` değişkenle eş zamanlı yönetme) bu teslimde **yer almamaktadır.**

---

## ✨ Özellikler

- ✅ Herhangi bir dosyadan veya standart girdiden (`stdin`) okuma yapabilir.
- ✅ Derleme anında `-D BUFFER_SIZE=n` bayrağı ile ayarlanabilen değişken tampon boyutu desteği (bayrak verilmezse varsayılan olarak `42` kullanılır).
- ✅ Dosya `\n` ile bitmese dahi son satırı kayıpsız döndürür.
- ✅ Dosya sonuna (`EOF`) ulaşıldığında veya bir okuma hatası oluştuğunda `NULL` döndürür.
- ✅ Yalnızca izin verilen `read`, `malloc` ve `free` fonksiyonlarını kullanır.
- ✅ `libft`, `lseek()` ve global değişken **kullanılmamıştır.**
- ✅ Bellek yönetimi baştan sona (partial/leftover veriler dâhil) `free()` ile temizlenir; memory leak hedeflenmemiştir.

---

## 📁 Proje Yapısı

```
.
├── get_next_line.c         # Ana fonksiyon: get_next_line() + akış kontrol fonksiyonları
├── get_next_line.h         # Prototipler ve BUFFER_SIZE tanımı
├── get_next_line_utils.c   # Yardımcı (utility) fonksiyonlar
└── README.md
```

| Dosya                       | İçerik                                                                 |
|------------------------------|-------------------------------------------------------------------------|
| `get_next_line.c`            | `get_next_line()`, `read_file()` (static), `flush_chunk()` (static)     |
| `get_next_line_utils.c`      | `line_len()`, `extract_line()`, `clean_stash()`, `join_stash()`         |
| `get_next_line.h`            | `BUFFER_SIZE` varsayılan tanımı (42) ve tüm fonksiyon prototipleri      |

---

## ⚙️ Kurulum ve Kullanım Talimatları

### Gereksinimler

- Bir C derleyicisi (`cc`, `gcc` veya `clang`)
- POSIX uyumlu bir ortam (Linux, macOS)

### Derleme

Proje bağımsız `.c` dosyalarından oluştuğu için ayrı bir `Makefile` gerektirmeden, doğrudan `cc` ile derlenip başka bir programa dahil edilebilir. Subject'te belirtilen standart derleme komutu:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl_test
```

`-D BUFFER_SIZE=n` bayrağı **isteğe bağlıdır**; verilmezse `get_next_line.h` içinde tanımlı varsayılan değer (**42**) kullanılır:

```c
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
```

Farklı tampon boyutlarıyla test etmek için:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1     get_next_line.c get_next_line_utils.c main.c -o gnl_1
cc -Wall -Wextra -Werror -D BUFFER_SIZE=9999  get_next_line.c get_next_line_utils.c main.c -o gnl_9999
cc -Wall -Wextra -Werror                      get_next_line.c get_next_line_utils.c main.c -o gnl_default
```

### Kullanım Örneği

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int   fd;
    char  *line;

    fd = open("dosya.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

Standart girdiden (`stdin`, fd = 0) okumak için de aynı fonksiyon doğrudan kullanılabilir:

```c
char *line;

while ((line = get_next_line(0)) != NULL)
{
    printf("Okunan: %s", line);
    free(line);
}
```

### Fonksiyon Prototipi

```c
char *get_next_line(int fd);
```

| Parametre | Açıklama                              |
|-----------|-----------------------------------------|
| `fd`      | Okunacak dosyanın betimleyicisi (descriptor) |

| Dönüş Değeri     | Anlamı                                                       |
|------------------|----------------------------------------------------------------|
| Okunan satır      | Başarılı okuma (satır, `\n` dahil, EOF'ta `\n` yoksa hariç) |
| `NULL`           | Okunacak veri kalmadığında ya da bir hata oluştuğunda        |

---

## 🧠 Algoritma: Detaylı Açıklama ve Gerekçelendirme

Bu bölüm, subject'in V. Bölümünde ("Readme Requirements") özellikle istenen **"seçilen algoritmanın detaylı açıklaması ve gerekçelendirmesi"** maddesini karşılamak amacıyla hazırlanmıştır.

### 1. Genel Yaklaşım: "İki katmanlı biriktirme" (two-level buffering)

Klasik `get_next_line` çözümlerinin çoğu, her `read()` çağrısından sonra doğrudan `stash` (kalıcı depo) üzerinde `malloc` + `memcpy` + `free` işlemi yaparak veriyi büyütür. `BUFFER_SIZE` küçük olduğunda (örn. varsayılan değer `42`, ya da uç durum `1`), bu yaklaşım **çok sayıda küçük `malloc`/`free` çağrısına** yol açar ve performansı ciddi şekilde düşürür.

Bu projede seçilen algoritma, bu maliyeti azaltmak için **iki katmanlı bir biriktirme stratejisi** uygular:

1. **Katman 1 — Geçici tampon (`buf`):** Her `get_next_line()` çağrısında, boyutu `b_size = max(4096, BUFFER_SIZE)` olan geçici bir yığın (heap) tamponu ayrılır. `read_file()` fonksiyonu bu tampon dolana veya içinde bir `\n` görülene kadar art arda `read(fd, buf + t, BUFFER_SIZE)` çağırarak veriyi bu tampona doldurur.
2. **Katman 2 — Kalıcı depo (`stash`):** Yalnızca aşağıdaki iki durumdan biri gerçekleştiğinde, geçici tamponun tamamı `join_stash()` ile kalıcı `stash`'e **tek seferde** eklenir:
   - geçici tampon, taşmayı önlemek için ayrılan sınıra yaklaştığında (`*t + BUFFER_SIZE > b_size`), **veya**
   - geçici tampon içinde bir satır sonu (`\n`) bulunduğunda (bu durumda döngü hemen durur, çünkü artık en az bir tam satır elde edilmiştir).

Bu tasarımın **gerekçesi**: `BUFFER_SIZE` çok küçük bir değerle (örneğin `1`) çalıştırıldığında bile, kalıcı `stash` üzerinde yapılan pahalı `join_stash` (malloc + çift kopyalama + free) işlemi **her `read()` çağrısında değil**, yalnızca "yaklaşık her 4096 baytta bir" veya "bir satır tamamlandığında" tetiklenir. Böylece `read()` çağrı sayısı `BUFFER_SIZE`'a bağlı kalmaya devam ederken (subject'in "mümkün olduğunca az oku" kısıtına uyulur), `stash` büyütme maliyeti önemli ölçüde azaltılmış olur.

### 2. Akış Diyagramı

```
get_next_line(fd)
    │
    ├─ fd < 0  veya  BUFFER_SIZE <= 0  →  NULL döndür
    │
    ├─ b_size = max(4096, BUFFER_SIZE) hesapla, buf ayır
    │
    ├─ read_file(fd, stash, buf, b_size)
    │     │
    │     └─ döngü:
    │           ├─ read(fd, buf + t, BUFFER_SIZE)
    │           ├─ b <= 0 ise döngüden çık
    │           ├─ t += b
    │           └─ flush_chunk(): buf dolmak üzereyse veya
    │                             buf içinde '\n' varsa
    │                             → join_stash(stash, buf)
    │                             → '\n' bulunduysa döngüyü durdur
    │
    ├─ okuma hatası oluştuysa (read() == -1) → stash'i free et, NULL döndür
    │
    ├─ line = extract_line(stash)   → ilk satırı (​\n dahil) kopyala
    ├─ stash = clean_stash(stash)   → ilk satırı stash'ten çıkar, kalanı sakla
    │
    └─ line döndür
```

### 3. `static` Değişkenin Rolü

```c
char *get_next_line(int fd)
{
    static char *stash;
    ...
}
```

`stash`, fonksiyonun **`static` yerel değişkenidir**: fonksiyon her çağrıldığında sıfırlanmaz, önceki çağrıdan kalan değerini korur. Bu, subject'in özellikle vurguladığı öğrenme hedefidir — global değişken kullanmadan, çağrılar arası durumu (state) saklamanın C dilindeki standart yoludur. Depoda henüz işlenmemiş (bir sonraki satıra veya satırlara ait) veri tutulur ve her çağrıda güncellenir.

### 4. Yardımcı Fonksiyonların Rolü

- **`line_len(str)`** — Bir karakter dizisinde ilk `\n` karakterine kadar (dahil) olan uzunluğu, `\n` yoksa dizinin tam uzunluğunu döndürür. Bu fonksiyon iki amaçla kullanılır: (1) bir satırın gerçek uzunluğunu bulmak, (2) bir tamponun içinde satır sonu olup olmadığını dolaylı yoldan tespit etmek.
- **`extract_line(stash)`** — `stash`'in başındaki ilk satırı (`line_len` kadar) yeni bir bellek alanına kopyalayarak döndürür; çağırana verilecek olan asıl satırdır.
- **`clean_stash(stash)`** — İlk satırı çıkardıktan sonra kalan veriyi yeni bir belleğe taşır, eski `stash`'i `free` eder. Kalan veri yoksa (`stash` tamamen tüketildiyse) `NULL` döndürerek bir sonraki çağrının "okunacak veri yok" durumuna doğru ilerlemesini sağlar.
- **`join_stash(stash, buf)`** — `stash` (kalıcı depo, ilk çağrıda `NULL` olabilir) ile `buf` (yeni okunan veri) içeriklerini tek bir yeni bellek bloğunda birleştirir, eski `stash`'i `free` eder.

### 5. Karmaşıklık (Complexity)

- **Zaman karmaşıklığı:** Dosya boyutu `n` olmak üzere, her bayt en fazla sabit sayıda kez kopyalanır (geçici tampona bir kez, kalıcı depoya bir kez, satır çıkarılırken bir kez) → toplamda **O(n)**.
- **Alan karmaşıklığı:** En kötü durumda (tek bir satırın tüm dosyayı kapsaması) **O(n)**; genel kullanımda ise en fazla `max(4096, BUFFER_SIZE)` + o anki satırın uzunluğu kadar bellek kullanılır.

### 6. Uç Durumların (Edge Case) Ele Alınışı

| Durum                                              | Davranış                                                            |
|-----------------------------------------------------|-------------------------------------------------------------------|
| `BUFFER_SIZE = 1`                                   | Çalışır; her `read()` 1 bayt okur, geçici tampon 4096 bayta kadar biriktirir. |
| `BUFFER_SIZE` çok büyük (örn. `9999`, `10000000`)   | Çalışır; `b_size = BUFFER_SIZE` olarak alınır, geçici tampon buna göre büyütülür. |
| Dosya `\n` ile bitmiyorsa                           | Son satır `\n` içermeden, dosyanın kalan tüm karakterleriyle döndürülür. |
| Boş dosya / dosya sonu                              | `NULL` döndürülür.                                                  |
| `read()` hata döndürürse (`-1`)                     | `stash` serbest bırakılır, `NULL` döndürülür.                       |
| `fd < 0`                                            | Hiçbir okuma yapılmadan `NULL` döndürülür.                          |
| Standart girdi (`stdin`, fd 0)                      | Diğer dosya betimleyicileriyle aynı şekilde çalışır (özel bir kod yolu yoktur). |

---

## 📚 Fonksiyon Referansı

### `get_next_line.c`

| Fonksiyon | İmza | Görev |
|---|---|---|
| `get_next_line` | `char *get_next_line(int fd);` | Dışa açık ana fonksiyon; bir sonraki satırı döndürür. |
| `read_file` *(static)* | `static char *read_file(int fd, char *stash, char *buf, int b_size);` | En az bir tam satır elde edilene ya da dosya sonuna gelinene kadar okuma döngüsünü yönetir. |
| `flush_chunk` *(static)* | `static int flush_chunk(char **stash, char *buf, int *t, int b_size);` | Geçici tamponu ne zaman kalıcı depoya aktaracağına karar verir. |

### `get_next_line_utils.c`

| Fonksiyon | İmza | Görev |
|---|---|---|
| `line_len` | `int line_len(char *str);` | İlk `\n`'e kadar (dahil) veya tam dize uzunluğunu hesaplar. |
| `extract_line` | `char *extract_line(char *stash);` | Depodaki ilk satırı ayrı bir bellek alanına kopyalar. |
| `clean_stash` | `char *clean_stash(char *stash);` | İlk satır çıkarıldıktan sonra kalan veriyi yeniden düzenler. |
| `join_stash` | `char *join_stash(char *stash, char *buf);` | Depo ile yeni okunan veriyi birleştirir. |

---

## 🚫 Kısıtlamalar ve Norm Uyumu

Subject'te belirtilen yasaklara bu projede uyulmuştur:

- ❌ `libft` **kullanılmamıştır** (`get_next_line.h` yalnızca `<stdlib.h>` ve `<unistd.h>` içerir).
- ❌ `lseek()` **kullanılmamıştır.**
- ❌ Global değişken **kullanılmamıştır** (yalnızca fonksiyon içi `static` değişken).
- ✅ Yalnızca `read`, `malloc`, `free` dış fonksiyonları kullanılmıştır.
- ✅ Kod, 42 Norm kurallarına (fonksiyon başına en fazla 25 satır, değişken tanımlarının blok başında yapılması, atama içeren tanım kullanılmaması, fonksiyon başına en fazla 4 parametre, dosya başlığı, vb.) uygun olacak şekilde yazılmıştır.

---

## ⚠️ Undefined Behavior ve Bilinen Sınırlamalar

Subject'in de belirttiği gibi, aşağıdaki durumlar tanımsız davranış (undefined behavior) kapsamındadır ve bu projede özel olarak ele alınmamıştır:

- Bir dosya betimleyicisine ait dosya, `get_next_line()` çağrıları arasında **`read()` dosya sonuna ulaşmadan** değiştirilirse.
- İkili (binary) dosyaların okunması.

Ayrıca:

- Bu teslim yalnızca **zorunlu (mandatory)** kısmı kapsar; birden fazla dosya betimleyicisinin tek bir `static` değişkenle eşzamanlı ve karışmadan yönetilmesi (bonus gereksinimi) bu sürümde **desteklenmemektedir.** Aynı anda birden fazla `fd` üzerinde dönüşümlü çağrı yapılırsa veriler karışabilir.
- `join_stash()` içindeki bellek boyutu hesaplaması `line_len()` fonksiyonuna dayanmaktadır. Büyük `BUFFER_SIZE` değerleriyle ve çok satırlı içerikle test ederken (örn. `BUFFER_SIZE=9999` gibi subject'in önerdiği uç değerler), bu noktayı kendi test dosyalarınızla ayrıca doğrulamanız önerilir.

---

## 🧪 Test Önerileri

Subject'in VII. Bölümü'nde belirtildiği gibi:

- Hem çok küçük (`1`) hem çok büyük (`9999`, `10000000`) `BUFFER_SIZE` değerleriyle test edin.
- Çok kısa ve çok uzun satırlar içeren dosyalarla test edin.
- Boş dosya, tek satırlık dosya, `\n` ile bitmeyen dosya senaryolarını test edin.
- Yalnızca normal dosyalarla değil, `stdin` gibi düzenli olmayan (non-regular) betimleyicilerle de test edin.
- `valgrind` ile bellek sızıntısı (memory leak) kontrolü yapın:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./gnl_test
```

---

## 📖 Kaynaklar

### Dokümantasyon ve Referanslar

- [`read(2)` — Linux man-pages](https://man7.org/linux/man-pages/man2/read.2.html)
- [`malloc(3)` / `free(3)` — Linux man-pages](https://man7.org/linux/man-pages/man3/malloc.3.html)
- C Standardı — `static` depolama süresi (storage duration) kavramı, K&R *The C Programming Language*.
- 42 Kocaeli — `get_next_line` proje subject dosyası (v14.3).
- [Norminette (42 Norm) dokümantasyonu](https://github.com/42School/norminette)

### AI Kullanımı Açıklaması

> ⚠️ **Bu bölüm, subject'in V. Bölümü'nde ("Readme Requirements") zorunlu tutulan, AI kullanımının hangi görevler ve hangi kısımlar için yapıldığının açıklanması gerekliliğini karşılamak üzere doldurulmalıdır. Aşağıdaki taslak bir başlangıç noktasıdır; kendi sürecinizi dürüstçe yansıtacak şekilde düzenleyiniz.**

- Bu **README.md dosyasının** yapılandırılması, subject gereksinimleriyle birebir eşleştirilmesi ve mevcut `get_next_line.c` / `get_next_line_utils.c` / `get_next_line.h` kaynak kodunun analiz edilip anlaşılır biçimde açıklanması için bir AI asistanından (Claude) yardım alınmıştır.
- Kaynak kodun (get_next_line.c, get_next_line_utils.c) tasarımı ve yazımı sürecinde doğrudan kod üretimi veya algoritma tasarımı için herhangi bir yapay zeka aracı kullanılmamıştır. AI, yalnızca projeye hazırlık aşamasında araştırmalar yapmak, statik değişkenler ve bellek yönetimi gibi kavramların teorik olarak anlaşılması amacıyla sınırlandırılmış bir şekilde kullanılmıştır. Kodun yazımı, kurgusu ve hata ayıklama süreçlerinin tamamı bizzat tarafımca gerçekleştirilmiştir.

---

## 👤 Yazar

| Login | Kampüs | E-posta |
|---|---|---|
| **muhaoz** | 42 Kocaeli | muhaoz@student.42kocaeli.com.tr |


---