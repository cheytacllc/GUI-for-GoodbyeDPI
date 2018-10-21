# GoodByeDPI GUI ve dnscrypt

[GoodByeDPI](https://github.com/ValdikSS/GoodbyeDPI) isimli anti sansür uygulaması için kullanıcı arayüzü ile kolay kullanım sağlar.

> İndirme Bağlantısı (Artık 32 ve 64 bit ayrımı yok!)
>
>[32 ve 64 Bit için İndir!](https://github.com/cheytacllc/GUI-for-GoodbyeDPI/releases/download/1.0.9/GoodByeDPI_GUI.zip)
>


# Kullanım

**Uygulamayı adında boşluk olan dizinlere çıkarmayın, çalıştırdığınızda hata verecektir**
Parametrelerin tam olarak kayıt edilebilmesi için Ayarlar penceresini **kapatıp** çalıştırmanız gerekli. Sonraki güncellemede eklenecek. Ana ekrandaki **Log** bölümünden uygulamanın düzgün çalışıp çalışmadığını kontrol edebilirsiniz, herhangi bir problem yaşadığınızda bildirmekten çekinmeyin.

> Programı kullanmak için VC++ 2015 x86 paketini yüklemeniz gerekmektedir. Dilerseniz programın dizinindeki **vc_redist.x86.exe** kurabilir ya da alttaki linkten indirebilirsiniz.
>
> Link: [Visual C++ 2015](https://www.microsoft.com/tr-TR/download/details.aspx?id=52685)

Başlangıçta özel bir ayar yapmanıza gerek yoktur. **Başlat** butonuna tıklayıp sansürsüz internetin keyfini çıkarabilirsiniz. Özel Ayarlar isteğe bağlı şekilde düzenlenebilir. İlk açılışta en uygun ayar **Ön Tanımlı** olarak ayarlanmıştır. 

> **İpucu: Ayarlar > Özel Parametre Kullan > Hızlı Ayarlar > [-3] -p -r -s -e 40 (better speed for HTTP and HTTPS)** seçeneğini kullandığınızda engel kalkıyorsa muhtemelen bağlantı hızınız daha iyi olacaktır.

> **Not:** ISPniz **Turknet** ise açamadığınız siteler varsa tarayıcınızda HTTPS zorlaması yapan **HTTPS Everywhere** gibi bir eklenti kullanarak sorundan kurtulabilirsiniz.

~~Windows DNS adresini kendiniz değiştirmeniz gerekmektedir. Aksi halde kuvvetle muhtemel engellenmiş sitelere erişim sağlayamayacaksınız.~~ **Artık DNS adresini otomatik değiştiriyor**

## Kaynak Koddan Derleme

Yazılım MSVC-2017(64Bit Versiyon) ve MinGW-5.3.0 ve MSVC2015(32Bit Versiyon) versiyonları ile derlenip **test** edilmiştir. Sadece bu derleyicileri kullanma zorunluluğu yoktur. Qt Framework Online Installer ile kurulum gerçekleştirirken, **MSVC ve MinGW derleyicilerinin yüklenmesi için işaretlemeyi unutmayın**.

Qt Creator'ı başlattıktan sonra File - Open File tıklayıp **.pro** uzantılı dosyayı seçin. Sonraki ekranda Configure butonuna basarak derleyiciniz için gerekli ayarlamaları yaptıktan sonra Build butonuna basarak yazılımı derleyebilirsiniz. Ardından Debug veya Release dizininde windeployqt uygulaması ile yazılımın çalışabilmesi için gerekli olan Qt kütüphanelerini otomatik şekilde kopyalayabilirsiniz.

## GoodByeDPI'ı Kaynak Koddan Derleme

> [Google Docs](https://docs.google.com/document/d/1LMGmFVu17NKItqTpJKGKXMhX58xWcCJPezddCo73e7c/edit?usp=sharing)

## İleriki Sürümlerde Eklenmesi Planlanan/Beklenen Özellikler
|Özellik| İyileştirme |
|--| --|
| Çalışma Saatleri Ayarlama (Tamamlandı) |Parametre Sisteminin Yeniden Kodlanması|
| Güncelleme Yöneticisi  |Daha İyi Hata Tespiti|
| Sistem DNS'i Değiştirebilme(Tamamlandı)|Arayüzde Yapılacak Değişiklikler |
| DnsCrypt(Tamamlandı)|  |
