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
	 
