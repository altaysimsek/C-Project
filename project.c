#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#define sorulacakSoru 5

//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BYEL "[01;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

//reset
#define reset "\e[0m"

//Sorular için struct yapýsýnýn tasarýmý
struct questions{
	int id;
	char desc[1024];
	char optA[50];
	char optB[50];
	char optC[50];
	char optD[50];
	char rOpt[50];
}sorular[100];
struct scores{
	int puan;
	char userName[50];
}highScore[100];
/*
Function createMenu()==>

1-Oyuna Basla
2-En yuksek skorlar
3-Sorularý düzenle
4-Ayarlar.
0-Cikis

Function createMenuSoru()==>

1-Sorulari goster
2-Soru ekle
3-Soru sil
*/
//Fonksiyon prototipleri --
void cleanConsole();
void createMenu();
void createMenuSoru();
void soruGoster(int soruSayisi);
void dosyaOku();
void soruEkle(int soruSayisi);
int sorulariGetir();
int soruBul(int id,int soruSayisi);
void startGame(int soruSayisi,char kullaniciAdi[]);
void saveScore(int score,char kullaniciAdi[]);
void soruSil(int index,int soruSayisi);
void updateSoru(int index,int soruSayisi,int temp);
int readHighScores();
void bubbleSort(int scoreSayisi);



int main(){
	SetConsoleTitle("LS[ASF] - Bil Kazan ");
	//sorular.txt okunur ve struct deðerleri belirlenir.
	int soruSayisi = sorulariGetir();
	int skorSayisi = readHighScores();//Sorulari okur.
	//Menü gösterilmeye baþlar.
	int menuSelection=1;
	int skor = 0;
	while (menuSelection != 0){
		cleanConsole();//console 'u temizler.
		createMenu();
		scanf("%d",&menuSelection);
		if (menuSelection == 1){
			//samet koda baþlar
			soruSayisi = sorulariGetir();
			cleanConsole();
			char kullaniciAdi[50];
			printf("=-=-=-==-=-=-==-=-=-==-=-=-==-=-=-==-=-=-==-=-=-==-=-=-==-=-=-==-=-=-==-=-=-==-=-=-==-=-=-=\n");
			printf(YEL"\t\t\t\tBir ad giriniz : "reset);
			scanf("%s",kullaniciAdi);
			cleanConsole();
			if(soruSayisi >5){
				startGame(soruSayisi,kullaniciAdi);	
			}else{
				cleanConsole();
				printf("Yeterli sayida soru yok.");
				getch();
			}
			
			//
		}else if (menuSelection == 2){
			cleanConsole();
			skorSayisi = readHighScores();
			bubbleSort(skorSayisi);
			printf(MAG"################################- En Yuksek Skorlar -####################################\n"reset);
			printf(YEL"\t\t\t\t1.%d puan ile %s\n\n"reset,highScore[skorSayisi-1].puan,highScore[skorSayisi-1].userName);
			printf(CYN"\t\t2.%d puan ile %s\n\n"reset,highScore[skorSayisi-2].puan,highScore[skorSayisi-2].userName);
			printf("\t\t\t\t\t\t3.%d puan ile %s\n",highScore[skorSayisi-3].puan,highScore[skorSayisi-3].userName);
			printf("\n\n\n\t\t\t\t4.%d puan ile %s\n",highScore[skorSayisi-4].puan,highScore[skorSayisi-4].userName);
			printf("\t\t\t\t5.%d puan ile %s\n",highScore[skorSayisi-5].puan,highScore[skorSayisi-5].userName);
			printf(MAG"##########################################################################################\n"reset);
			printf("\n\nDevam etmek icin bir tusa basin.");
			
			getch();
		}else if (menuSelection == 3){
			cleanConsole();
			createMenuSoru();
			soruSayisi = sorulariGetir();
			int soruSelection = 1;
			scanf("%d",&soruSelection);
			if (soruSelection == 1){
				soruSayisi = sorulariGetir();
				soruGoster(soruSayisi);
				getch();
			}else if (soruSelection == 2){
				soruEkle(soruSayisi);
				printf(CYN"Devam etmek icin bir tusa basin"reset);
				getch();
				
			}else if (soruSelection == 3){
				cleanConsole();
				soruSayisi = sorulariGetir();
				printf(RED"################################- Silinecek SORU -####################################\n"reset
				"Soru id'sini giriniz : ");
				int temp = 0;
				scanf("%d",&temp);
				int silinecekIndis = soruBul(temp,soruSayisi);
				soruSil(silinecekIndis,soruSayisi);
				getch();
				
			}else if (soruSelection == 4){
				cleanConsole();
				soruSayisi = sorulariGetir();
				printf(RED"################################- Guncellenecek SORU -####################################\n"reset
				"Soru id'sini giriniz : ");
				int temp = 0;
				scanf("%d",&temp);
				int updatedIndex = soruBul(temp,soruSayisi);
				updateSoru(updatedIndex,soruSayisi,temp);
				getch();
				
			}else if (soruSelection == 0){
				
			}else{
			}
		}else if (menuSelection == 0){
			exit(1);
		}else{
			printf(RED"\t\t\t\tHatali islem."reset);
			sleep(1);
		}
	}			
}

//Fonksiyonlar
void cleanConsole(){
	system("@cls||clear");
}
void createMenu(){
	printf(GRN"\t\t\t\t\t-=Kim Milyoner Olmak Ister =-\n"reset);
	printf(YEL"\t\t\t\t-Lutfen asagidan yapmak istediginiz islemi secin.-\n"reset);
	printf("\t\t\t\t1-Oyuna Basla ! \n\t\t\t\t2-En yuksek skorlar ! \n\t\t\t\t3-Sorular\n\t\t\t\t"RED"0-Cikis yap.\n"reset);
	printf("\t\t\t\tIsleminiz -> ");
}
void createMenuSoru(){
	printf(GRN"\t\t\t\t\t-= Sorular bolumune hosgeldin. =-\n"reset);
	printf(YEL"\t\t\t\t-Lutfen asagidan yapmak istediginiz islemi secin.-\n"reset);
	printf("\t\t\t\t1-Sorulari goster. \n\t\t\t\t2-Soru ekle \n\t\t\t\t3-Soru sil\n\t\t\t\t4-Soru guncelle\n\t\t\t\t"RED"0-Geri don.\n"reset);
	printf("\t\t\t\tIsleminiz -> ");
}
void soruGoster(int soruSayisi){
	cleanConsole();
	int localCounter = 1;
	printf(RED"################################- Sorularin Bilgileri -####################################\n"reset);
	while(localCounter < soruSayisi){
		
		if((sorular[localCounter].id == 101) || (sorular[localCounter].id == 0)){
			localCounter++;
			continue;
		}else{
			printf("\nSoru ID : %d\nSoru aciklamasi : %s\nA_Sikki : %s\nB_sikki : %s\nC_sikki : %s\nD_sikki : %s\nDogru Cevap : %s\n",sorular[localCounter].id,sorular[localCounter].desc,sorular[localCounter].optA,sorular[localCounter].optB,sorular[localCounter].optC,sorular[localCounter].optD,sorular[localCounter].rOpt);
			localCounter++;
		}
		
	}
	printf(RED"################################- 2019-2020 -##############################################\n"reset);
	printf(CYN"\nBir tusa basarak cikin."reset);
}
void soruEkle(int soruSayisi){
	cleanConsole();
	struct questions soruEkle;
	FILE *file;
	file = fopen("sorular.txt","a");
	srand(time(0));
	int idUret = 1+(rand() % 100);
	int lCounter = 0;
	printf(MAG"Bir sure bekleyin - Soru bilgileri isleniyor.\n"reset);
	while (lCounter <= soruSayisi){

		if(idUret == sorular[lCounter].id){
			srand(time(0));
			idUret = 1+(rand() % 100);
			lCounter = 0;
		}else{
			if(lCounter == soruSayisi){
				break;
			}
			lCounter++;
		}
		if(lCounter == soruSayisi){
			
		}
	}	
	soruEkle.id = idUret;
	printf(RED"################################- Sorularin Bilgileri -####################################"reset);
	printf("\nA_sikki : ");
	scanf("%s",soruEkle.optA);
	printf("\nB_sikki : ");
	scanf("%s",soruEkle.optB);
	printf("\nC_sikki : ");
	scanf("%s",soruEkle.optC);
	printf("\nD_sikki : ");
	scanf("%s",soruEkle.optD);
	printf("\nDogru cevap : ");
	scanf("%s",soruEkle.rOpt);
	getchar();
	printf("\nSorunuz : ");
	gets(soruEkle.desc);
	fprintf(file,"\n%d\t%s\t%s\t%s\t%s\t%s\t%s\t",soruEkle.id,soruEkle.optA,soruEkle.optB,soruEkle.optC,soruEkle.optD,soruEkle.rOpt,soruEkle.desc);
	printf(RED"################################- 2019-2020 -##############################################\n"reset);
	printf("-Sorunuz basari ile eklendi...\n");
	fclose(file);
	
}
//son degisme 8.12.2019
int sorulariGetir(){
	FILE *file;
	file = fopen("sorular.txt","r");
	int soruSayisi = 0;//böyle bir degisken tanimlanir, cünkü soru idsi otomatik belirlenmeli.
	if (file != NULL){
		while(!feof(file)){
			fscanf(file,"%d\t%s\t%s\t%s\t%s\t%s\t",&sorular[soruSayisi].id,&sorular[soruSayisi].optA,&sorular[soruSayisi].optB,&sorular[soruSayisi].optC,&sorular[soruSayisi].optD,&sorular[soruSayisi].rOpt);
			fgets(sorular[soruSayisi].desc,1024,file);//rOpt deðeri okunduktan sonra geri kalan yatay uzunluk soru aciklamasýný verir.
			soruSayisi++;
		}
		
	}else{
		printf("\t\t\t\t-= sorular.txt acilamadi,3sn sonra menuye aktarilacaksin. =-\n");
		file = fopen("sorular.txt","w");
		fprintf(file,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t",0,"A_cevabi","B_cevabi","C_cevabi","D_cevabi","Dogru cevap","Soru aciklamasi budur");
		sleep(3);
	}
   	fclose(file);
   	return soruSayisi;
}
int soruBul(int id,int soruSayisi){
	int lCounter = 1;
	while (lCounter <= soruSayisi){
		if (id == sorular[lCounter].id){
			return lCounter;
		}else{
			lCounter++;
		}
	}
}
void startGame(int soruSayisi,char kullaniciAdi[]){
	int score = 0;
	int sayilar[5]={0};
	srand(time(NULL));
	int rastgeleIndis;
	rastgeleIndis = 1+(rand() % (soruSayisi-1));
	int i = 1;
	cleanConsole();
	for (i = 1; i<=sorulacakSoru;i++){
		char cevap;
		cleanConsole();
		printf(CYN"Soru getiriliyor."reset);
		sleep(1);
		int ctLoop = 0;
		for(ctLoop = 0;ctLoop <= 4;ctLoop++){
			if (rastgeleIndis == sayilar[ctLoop]){
				srand(time(NULL));
				rastgeleIndis = 1+(rand()%(soruSayisi-1));
				ctLoop = -1;				
			}else{
				continue;
			}
		}
		printf(CYN"."reset);
		sayilar[i-1] = rastgeleIndis;
		cleanConsole();
		printf(CYN"################################- 2019-2020 -##############################################\n"reset);
		printf("\t\t\t\t[%d / 5]\n",i);
		printf("\t\t\tSorunuz : %s\n",sorular[rastgeleIndis].desc);
		printf("\t\tA ) %s\tB ) %s\tC ) %s\tD ) %s\n",sorular[rastgeleIndis].optA,sorular[rastgeleIndis].optB,sorular[rastgeleIndis].optC,sorular[rastgeleIndis].optD);
		printf(CYN"################################- 2019-2020 -##############################################"reset);
		printf(RED"\nUyari : Kucuk buyuk harf duyarlidir.\n"reset);
		printf("\nCevabiniz : ");
		time_t startClock = time(NULL);
		getchar();
		scanf("%c",&cevap);
		time_t endClock = time(NULL);
		int gecenZ = difftime(endClock,startClock);
		if(cevap == 'A'){
			if(!strcmp(sorular[rastgeleIndis].optA,sorular[rastgeleIndis].rOpt)){
				score += 100;
				
				if(gecenZ>3){
					
				}else{
					score+=((10-gecenZ)*100);
					
				}
				printf(GRN"\nDogru cevap %s"reset,sorular[rastgeleIndis].rOpt);
				printf(GRN"\nYeni soru icin herhangi bir tusa basin."reset,sorular[rastgeleIndis].rOpt);
				getch();
				
				
				continue;
			}else{
				printf(GRN"\nDogru cevap -> %s"reset,sorular[rastgeleIndis].rOpt);
				printf(GRN"\nYeni soru icin herhangi bir tusa basin."reset,sorular[rastgeleIndis].rOpt);
				getch();
				continue;
				
			}
		}else if (cevap == 'B'){
			if(!strcmp(sorular[rastgeleIndis].optB,sorular[rastgeleIndis].rOpt)){
				score += 100;
				
				if(gecenZ>3){
					
				}else{
					score+=((10-gecenZ)*100);
					
				}
				printf(GRN"\nDogru cevap %s"reset,sorular[rastgeleIndis].rOpt);
				printf(GRN"\nYeni soru icin herhangi bir tusa basin."reset,sorular[rastgeleIndis].rOpt);
				getch();
				continue;
			}else{
				printf(GRN"\nDogru cevap -> %s"reset,sorular[rastgeleIndis].rOpt);
				printf(GRN"\nYeni soru icin herhangi bir tusa basin."reset,sorular[rastgeleIndis].rOpt);
				getch();
				continue;
				
			}
		}else if (cevap == 'C'){
			if(!strcmp(sorular[rastgeleIndis].optC,sorular[rastgeleIndis].rOpt)){
				score += 100;
				
				if(gecenZ>3){
					
				}else{
					score+=((10-gecenZ)*100);
					
				}
				printf(GRN"\nDogru cevap %s"reset,sorular[rastgeleIndis].rOpt);
				printf(GRN"\nYeni soru icin herhangi bir tusa basin."reset,sorular[rastgeleIndis].rOpt);
				getch();
				continue;
			}else{
				printf(GRN"\nDogru cevap -> %s"reset,sorular[rastgeleIndis].rOpt);
				printf(GRN"\nYeni soru icin herhangi bir tusa basin."reset,sorular[rastgeleIndis].rOpt);
				getch();
				continue;
			}
		}else if(cevap == 'D'){
			if(!strcmp(sorular[rastgeleIndis].optD,sorular[rastgeleIndis].rOpt)){
				score += 100;
				
				if(gecenZ>3){
					
				}else{
					score+=((10-gecenZ)*100);
					
				}
				printf(GRN"\nDogru cevap -> %s"reset,sorular[rastgeleIndis].rOpt);
				printf(GRN"\nYeni soru icin herhangi bir tusa basin."reset,sorular[rastgeleIndis].rOpt);
				getch();
				continue;
			}else{
				printf(GRN"\nDogru cevap -> %s"reset,sorular[rastgeleIndis].rOpt);
				printf(GRN"\nYeni soru icin herhangi bir tusa basin."reset,sorular[rastgeleIndis].rOpt);
				getch();
				continue;
			}
		}
	
	}
	cleanConsole();
	printf(CYN"################################- 2019-2020 -##############################################\n"reset);
	printf("\n\t\t Tebrikler , %s\n",kullaniciAdi);
	printf("\n\t\t SKORUN : %d\n",score);
	printf(CYN"################################- 2019-2020 -##############################################\n"reset);
	saveScore(score,kullaniciAdi);
	printf("Devam etmek icin bir tusa basin.");
	getch();
}
void saveScore(int score,char kullaniciAdi[]){
	FILE *file;
	file = fopen("highscores.txt","a");
	if(file != NULL){
		fprintf(file,"%d\t%s\t",score,kullaniciAdi);
		
	}else{
		// dosya acilmadi ise.
	}
	fclose(file);
}
void soruSil(int index,int soruSayisi){
	FILE *file;
	if((file = fopen("sorular.txt","w"))!=NULL){
		int i = 0;
		for (i=0;i<soruSayisi;i++){
			if(i == index){
				printf("Basariyla silindi.\n");
				sorular[i].id = 101;
				soruSayisi--;
				continue;
			}else{
				printf("Yeniden yaziliyor.\n");
				printf("%d\t%s\t%s\t%s\t%s\t%s\t%s\n",sorular[i].id,sorular[i].optA,sorular[i].optB,sorular[i].optC,sorular[i].optD,sorular[i].rOpt,sorular[i].desc);
				fprintf(file,"%d\t%s\t%s\t%s\t%s\t%s\t%s",sorular[i].id,sorular[i].optA,sorular[i].optB,sorular[i].optC,sorular[i].optD,sorular[i].rOpt,sorular[i].desc);
			}
		}
	}else{
		printf("Dosya acilamadi.");
	}
	fclose(file);
}
void updateSoru(int index,int soruSayisi,int temp){
	soruSil(index,soruSayisi);
	 
	cleanConsole();
	struct questions soruEkle;
	FILE *file;
	file = fopen("sorular.txt","a");
		
	soruEkle.id = temp;
	printf(RED"################################- Sorularin Bilgileri -####################################"reset);
	printf("\nEski Deger -> %s | Yeni A cevabi : ",sorular[index].optA);
	scanf("%s",soruEkle.optA);
	printf("\nEski Deger -> %s | Yeni B cevabi : ",sorular[index].optB);
	scanf("%s",soruEkle.optB);
	printf("\nEski Deger -> %s | Yeni C cevabi : ",sorular[index].optC);
	scanf("%s",soruEkle.optC);
	printf("\nEski Deger -> %s | Yeni D cevabi : ",sorular[index].optD);
	scanf("%s",soruEkle.optD);
	printf("\nEski Deger -> %s | Yeni Dogru Cevap : ",sorular[index].rOpt);
	scanf("%s",soruEkle.rOpt);
	getchar();
	printf("\nEski Deger -> %s | Yeni Sorunuz : ",sorular[index].desc);
	gets(soruEkle.desc);
	fprintf(file,"\n%d\t%s\t%s\t%s\t%s\t%s\t%s\t",soruEkle.id,soruEkle.optA,soruEkle.optB,soruEkle.optC,soruEkle.optD,soruEkle.rOpt,soruEkle.desc);
	printf(RED"################################- 2019-2020 -##############################################\n"reset);
	printf("-Sorunuz basari ile guncellendi...\n");
	fclose(file);
	
}
int readHighScores(){
	FILE *file;
	file = fopen("highscores.txt","r");
	int scoreSayisi=0;
	if(file != NULL){
		while(!feof(file)){
			fscanf(file,"%d\t%s",&highScore[scoreSayisi].puan,&highScore[scoreSayisi].userName);
			scoreSayisi++;
		}
	}else{
		printf("Dosya okunamadý.\n");
		file = fopen("highscores.txt","w");
	}
	fclose(file);
	return scoreSayisi-1;
}
void bubbleSort(int scoreSayisi){
	int i = 0;
	int a = 0;
	for(i=0;i<scoreSayisi-1;i++){
		for(a=0;a<scoreSayisi-1;a++){
			if(highScore[a].puan > highScore[a+1].puan){
				int temp = highScore[a].puan;
				char tempName[50] ;
				strcpy (tempName,highScore[a].userName);
				highScore[a].puan = highScore[a+1].puan;
				strcpy (highScore[a].userName,highScore[a+1].userName);
				strcpy (highScore[a+1].userName,tempName);
				highScore[a+1].puan = temp;
			}
		}
	}
}







