[English](#goodbyedpi-gui-and-dnscrypt)  [Türkçe](#goodbyedpi-gui-ve-dnscrypt)

# GoodByeDPI GUI and dnscrypt

This application is user interface of [GoodByeDPI](https://github.com/ValdikSS/GoodbyeDPI) anti-censorship application for ease of use and adding some features.

> Download
>
>[Download!](https://github.com/mguludag/GUI-for-GoodbyeDPI/releases/download/1.1.9/GoodByeDPI_GUI.zip)
>


# Usage

**Don't extract the application in directories that have spaces in the name, it will give an error when you run the application.**
You need to **close the settings window** and hit start button so that the parameters can be saved. You can check whether the application works correctly from **log section** on the home screen, feel free to let me know when you have any problems.

> To use the program you need to install the VC++ 2017 x86 package. If you wish, you can install **vc_redist.x86.exe** in the directory of the program or download it from the link below.

>
> Link: [Visual C++ 2017](https://go.microsoft.com/fwlink/?LinkId=746571)

You don't need to make a custom setting at startup. Click on the **start** button and enjoy the uncensored internet. Custom settings can be optionally edited. The optimal settings is set to **default**.


> **Tip: Settings > Use Custom Parameters > Quick Settings > [-3] -p -r -s -e 40 (better speed for HTTP and HTTPS)** If you use the option, the connection will be better if the blocking is removed.

> **Note:** If your ISP is **Turknet** and you cannot open some sites you can use plugin like **HTTPS Ewerywhere** in your browser for get rid of that problem.

~~You must change the windows dns address yourself. otherwise you will not be able to access blocked sites.~~ **Now changing the dns address automatically**

## Compilation from source code

The software has been compiled with the versions of msvc-2017 (64bit version) and mingw-5.3.0 and msvc2015 (32bit version) **test**. it is not necessary to use these compilers only. When installing with qt framework online installer, do not forget to mark **for installation of MSVC and mingw compilers**.

After starting qt creator, select file > open file select the file extension is **.pro**. After clicking the configure button on the next screen, you can compile the software by clicking on the build button. then you can automatically copy the qt libraries that are necessary for the software to work with the windeployqt application in the debug or release directory.


## Compilation of GUI For GoodbyeDPI

> [Google Docs (Not translated yet)](https://docs.google.com/document/d/1LMGmFVu17NKItqTpJKGKXMhX58xWcCJPezddCo73e7c/edit?usp=sharing)

## Donate (Support)
>[Patreon](https://www.patreon.com/cheytacllc)

## New features/fixes planning/expecting for future releases
|Feature| Fix |
|--| --|
| Schedule start/stop times (completed) |Reimplementation of Parameter System|
| Updated (completed)  |Better error handling|
| Auto change system's DNS(completed)|Better UI |
| DnsCrypt(completed)|  |

## Kudos
Thanks to @ValdikSS for [GoodByeDPI](https://github.com/ValdikSS/GoodbyeDPI). This is core of the program.
Thanks to @jedisct1 for [dnscrypt-proxy](https://github.com/jedisct1/dnscrypt-proxy). This makes use of secure DNSCrypt servers.


# GoodByeDPI GUI ve dnscrypt

[GoodByeDPI](https://github.com/ValdikSS/GoodbyeDPI) isimli anti sansür uygulaması için kullanıcı arayüzü ile kolay kullanım sağlar.

> İndirme Bağlantısı (Artık 32 ve 64 bit ayrımı yok!)
>
>[32 ve 64 Bit için İndir!](https://github.com/mguludag/GUI-for-GoodbyeDPI/releases/download/1.1.9/GoodByeDPI_GUI.zip)
>


# Kullanım

**Uygulamayı adında boşluk olan dizinlere çıkarmayın, çalıştırdığınızda hata verecektir**
Parametrelerin tam olarak kayıt edilebilmesi için Ayarlar penceresini **kapatıp** çalıştırmanız gerekli. Sonraki güncellemede eklenecek. Ana ekrandaki **Log** bölümünden uygulamanın düzgün çalışıp çalışmadığını kontrol edebilirsiniz, herhangi bir problem yaşadığınızda bildirmekten çekinmeyin.

> Programı kullanmak için VC++ 2017 x86 paketini yüklemeniz gerekmektedir. Dilerseniz programın dizinindeki **vc_redist.x86.exe** kurabilir ya da alttaki linkten indirebilirsiniz.
>
> Link: [Visual C++ 2017](https://go.microsoft.com/fwlink/?LinkId=746571)

Başlangıçta özel bir ayar yapmanıza gerek yoktur. **Başlat** butonuna tıklayıp sansürsüz internetin keyfini çıkarabilirsiniz. Özel Ayarlar isteğe bağlı şekilde düzenlenebilir. İlk açılışta en uygun ayar **Ön Tanımlı** olarak ayarlanmıştır. 

> **İpucu: Ayarlar > Özel Parametre Kullan > Hızlı Ayarlar > [-3] -p -r -s -e 40 (better speed for HTTP and HTTPS)** seçeneğini kullandığınızda engel kalkıyorsa muhtemelen bağlantı hızınız daha iyi olacaktır.

> **Not:** ISPniz **Turknet** ise açamadığınız siteler varsa tarayıcınızda HTTPS zorlaması yapan **HTTPS Everywhere** gibi bir eklenti kullanarak sorundan kurtulabilirsiniz.

~~Windows DNS adresini kendiniz değiştirmeniz gerekmektedir. Aksi halde kuvvetle muhtemel engellenmiş sitelere erişim sağlayamayacaksınız.~~ **Artık DNS adresini otomatik değiştiriyor**

## Kaynak Koddan Derleme

Yazılım MSVC-2017(64Bit Versiyon) ve MinGW-5.3.0 ve MSVC2015(32Bit Versiyon) versiyonları ile derlenip **test** edilmiştir. Sadece bu derleyicileri kullanma zorunluluğu yoktur. Qt Framework Online Installer ile kurulum gerçekleştirirken, **MSVC ve MinGW derleyicilerinin yüklenmesi için işaretlemeyi unutmayın**.

Qt Creator'ı başlattıktan sonra File - Open File tıklayıp **.pro** uzantılı dosyayı seçin. Sonraki ekranda Configure butonuna basarak derleyiciniz için gerekli ayarlamaları yaptıktan sonra Build butonuna basarak yazılımı derleyebilirsiniz. Ardından Debug veya Release dizininde windeployqt uygulaması ile yazılımın çalışabilmesi için gerekli olan Qt kütüphanelerini otomatik şekilde kopyalayabilirsiniz.

## GoodByeDPI'ı Kaynak Koddan Derleme

> [Google Docs](https://docs.google.com/document/d/1LMGmFVu17NKItqTpJKGKXMhX58xWcCJPezddCo73e7c/edit?usp=sharing)

## Bağış (Support)
>[Patreon](https://www.patreon.com/cheytacllc)

## İleriki Sürümlerde Eklenmesi Planlanan/Beklenen Özellikler
|Özellik| İyileştirme |
|--| --|
| Çalışma Saatleri Ayarlama (Tamamlandı) |Parametre Sisteminin Yeniden Kodlanması|
| Güncelleme Yöneticisi (Eklendi)  |Daha İyi Hata Tespiti|
| Sistem DNS'i Değiştirebilme(Tamamlandı)|Arayüzde Yapılacak Değişiklikler |
| DnsCrypt(Tamamlandı)|  |
