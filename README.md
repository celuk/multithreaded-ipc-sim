# multithreaded-ipc-sim
Multithreaded Inter Process Communication Simulation

Programın amacı; ipc kullanarak sunucu ve istemci simülasyonu yaparak farklı threadler arası mesaj alışverişi yapmak.

Programı çalıştırmak için yapılması gerekenler;

**1-)** `server.c` ve `client.c` kodlarını aşağıdaki gibi derleyin.

```bash
gcc server.c -o server
gcc client.c -o client
```

**2-)** `./server` şeklinde server processi çalıştırın.

**3-)** `./client <client_id>` şeklinde client processleri (ayrı terminallerde) çalıştırın.

Örnek:

```bash
./client 1234
./client abc
```

**4-)** client terminallerinde sunucu üzerinden birbiri arasında mesaj göndermek için ilgili terminalde ekranda yazan talimatları uygulayın.

Aşağıda 2 client varken 3 ayrı terminalde gerçekleştirilen simülasyon örneği verilmiştir:

### SERVER TERMİNALİ ###

```bash
shc@karpuz:~$ ./server
Sunucu baslatiliyor...
Sunucuyu kapatmak icin CTRL+C tus kombinasyonunu kullanin.
1234 istemcisi icin worker thread olusturuluyor...
abc istemcisi icin worker thread olusturuluyor...
1234 istemcisinden alinan mesaj: deneme
1234 istemcisinden --> abc istemcisine gönderiliyor...
^Cshc@karpuz:~$
```

### CLIENT 1234 TERMİNALİ ###

```bash
shc@karpuz:~$ ./client 1234
1234 istemcisi baslatiliyor...
Yeni mesajlar bekleniyor...
Mesaj gondermek için 'y', cikmak icin 'c' tusuna ve daha sonra entera basin
y
Mesaj gondermek istediginiz istemcinin id'sini girin: abc
abc istemcisine gondermek istediginiz mesaji yazin: deneme
Yeni mesajlar bekleniyor...
Mesaj gondermek için 'y', cikmak icin 'c' tusuna ve daha sonra entera basin
c
shc@karpuz:~$
```

### CLIENT abc TERMİNALİ ###

```bash
shc@karpuz:~$ ./client abc
abc istemcisi baslatiliyor...
Yeni mesajlar bekleniyor...
Mesaj gondermek için 'y', cikmak icin 'c' tusuna ve daha sonra entera basin
1234 istemcisinden gelen mesaj: deneme
c
shc@karpuz:~$
```

