**Minishell**

> Bash, fish, zsh gibi kabukların basit bir implementasyonu. 

 - [ ] Yeni bir komut beklenirken prompt göstermeli.
 - [ ] Çalışan bir komut geçmişi (history) özelliğine sahip olmalı.
 - [ ] Alınan sinyalleri işlemek için, sadece sinyal numarasının saklandığı en fazla bir global değişken kullanmalı.
 - [ ] Özel karakterleri --(\, ;) gibi-- ve eksik tırnakları yorumlamamalı.
 - [ ] Tek tırnak ' içindeki meta karakterlerin yorumlanması engellenmeli.
 - [ ] Çift tırnak içindeki " metak karakterlerin -- $ işareti hariç -- yorumlanması engellenmeli.
 - [ ] >, >>, <<, <, | işaretleri desteklenmeli.
 - [ ] Çevresel değişkenleri $ ile görebilmeli.
 - [ ] Son çalıştırılan pipellinenın çıkış durumu $? ile gösterilmeli.
 - [ ] Ctrl-C, Ctrl-D Ctrl-\ bashtaki gibi işlemeli
	- Ctrl-C yeni satırda prompt gösterir.
	- Ctrl-D kabuktan çıkar.
	- Ctrl-\ hiçbir şey yapmaz.
 - [ ] Builtin komutları ayrı bir şekilde implemente etmeli.
	 - Echo / ekstra -n parametresi ile
	 - cd / relative, mutlak yol
	 - pwd
	 - export
	 - unset
	 - env
	 
------------------------------------

**Öğrenilmesi Gerekenler**

 - [ ] Readlines ile kullanıcıdan sürekli komut beklemek.
 - [ ] add_history / burada bence builtin history komutu implemente edersek daha hoş olur,
add_history ayrıca linkedlistli bir history olmalı, history komutu girince geçmişteki komutları görebilmeliyiz.
 - [ ] sigaction ile sinyal handllelama
 - [ ] tek tırnak, çift tırnak, gibi şeylerin bash üstünde tam olarak nasıl çalıştığını anlamak lazım
 - [ ] redirection ve pipe olayını tam olarak kavramak --> pipexi bitirince anlamış oluyoruz.
 - [ ] redirection ve pipeların birden çok olduğu komutlar için lexer/parser gerekiyor. yani her komutu bu şekilde
düşünüp bir işleyici oluşturmak gerekiyor.
 - [ ] $ işareti ve env handllelama
