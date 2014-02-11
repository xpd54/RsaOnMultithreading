//RSA encryption and decryption built by Ravi prakash(y10uc256),Rajat vyas(y10uc250)

#include<stdio.h>  	//Adding header files  
#include<stdlib.h>
#include<math.h>
#include<pthread.h>  
#define RANGE 400	//Range of prime numbers which will use in prime_value (p and q)
#define MAXIMUM 100000
struct encryption_value		
{
	long int public_key,private_key,phi,multiply;
	int plain;
};
long int gcd(long long unsigned int,long long unsigned int);
void *multiply_n(void *);
void *phi(void *);
void *value_e(void *);
void *table_method(void *);
void *calculate_dk(void *);
void *file_process(void *);
void *file_deprocess(void *);
long int encryption(long int,long int,long int);
long int decryption(long int,long int,long int);
void file_encryption(struct encryption_value);
void file_decryption(struct encryption_value);
int input_text[MAXIMUM],output_text[MAXIMUM];
struct prime_value
{
	long int p,q;
};

struct table_value
{
	long int phi,e;
};

struct equation_value
{
	long int d[1000],k[1000],a[1000],b[1000];
};
struct prime_value globle_prime;


main()
{
	struct prime_value prime;
	struct table_value table;
	struct encryption_value value;
	long int cipher,plain,real_value;
	pthread_t multiply_t,phi_t,encrypt_t,table_t;
	void *multiply_value, *phi_value, *public,*private,*prime_p,*prime_q;
	int max=RANGE,min=RANGE/2;
	prime_p=value_e((void *)max);
	prime_q=value_e((void *)min);
	printf("%d\n",(int)prime_p);
	printf("%d\n",(int)prime_q);
	prime.p=(int)prime_p;
	prime.q=(int)prime_q;
	printf("input the prime number\n");
	pthread_create(&multiply_t,NULL,multiply_n,(void *) &prime);
	pthread_create(&phi_t,NULL,phi,(void *) &prime);
	pthread_join(multiply_t,&multiply_value);
	pthread_join(phi_t,&phi_value);
	pthread_create(&encrypt_t,NULL,value_e,(void *) phi_value);
	pthread_join(encrypt_t,&public);
	table.phi = (long int)phi_value;
	table.e = (long int)public;
	pthread_create(&table_t,NULL,table_method,(void *) &table);
	pthread_join(table_t,&private);
	printf("test n=%ld phi=%ld   e=%ld  d=%ld \n",(long int)multiply_value,(long int)phi_value,(long int)public,(long int)private);
	value.public_key = (long int)public;
	value.private_key = (long int)private;
	value.phi = (long int)phi_value;
	value.multiply = (long int)multiply_value;
	file_encryption(value);
	file_decryption(value);
	
	
	
}
void *table_method(void *arg)
{
	struct table_value *table;
	struct equation_value column;
	long int phi,e,private;
	void *d;
	pthread_t k_t,d_t,a_t,b_t;
	table = (struct table_value *)arg;
	phi = table->phi;
	e = table->e;
	column.d[1]=phi;
	column.d[2]=e;
	column.a[1]=1;
	column.a[2]=0;
	column.b[1]=0;
	column.b[2]=1;
	pthread_create(&d_t,NULL,calculate_dk,(void *) &column);
	pthread_join(d_t,&d);
	private = (long int)d;
	if(private < 0)
	{
		private = private + phi;
	}
	if(private > phi)
	{
		private = private%phi;
	}
	return (void *)private;
}

void *calculate_dk(void *arg)
{
	struct equation_value *column;
	column = (struct equation_value *)arg;
	long int count=3,e_value;
	column->k[2]=(long int)(column->d[1]/column->d[2]);
	while(1)
	{
		
		column->d[count]=(long int)(column->d[count-2] - column->d[count-1]*column->k[count-1]);
		column->a[count]=(long int)(column->a[count-2] - column->a[count-1]*column->k[count-1]);
		column->b[count]=(long int)(column->b[count-2] - column->b[count-1]*column->k[count-1]);
		if(column->d[count]==1)
			break;
		column->k[count]=(long int)(column->d[count-1]/column->d[count]);
		count++;

	}
	e_value = column->b[count];
	return (void *)e_value;
}

void *multiply_n(void *arg)
{
	struct prime_value *prime;
	long int multiply_value;
	prime = (struct prime_value *)arg;
	multiply_value=prime->p * prime->q;
	return (void *)multiply_value;
}

void *phi(void *arg)
{
	struct prime_value *prime;
	long int phi_value;
	prime = (struct prime_value *)arg;
	phi_value=(prime->p -1) * (prime->q - 1);
	return (void *)phi_value;
}

void *value_e(void *arg)
{
    
    long int no,counter,counter1,check,test,prime_array[1000000],count=0,factor=2,skip_count;
    no = (long int) arg;
    double test_value;
    int flag=1;
    
    for(counter = 2; counter <= no ; counter++)
    {
        check = 0;
	test=(int)sqrt(counter);
        
		for(counter1=2; counter1 <=test && flag==1;)
		{
		
        		if(counter%counter1 == 0)
        		{
            			check++;
           			break;
       			}
			counter1++;
			if(counter1%2==0)
				{
					counter1++;
				}
			
		}
	
	if(check == 0)
	{
	prime_array[count]=counter;
	count++;
	factor=prime_array[count-1];
	}
    }
	do{
	
		srand(time(NULL));
		count=rand()%(count-3+1)+3;
	  }
	while(gcd(prime_array[count],no)!=1);
	return (void *)prime_array[count];

}




long int gcd(long long unsigned int num1,long long unsigned int num2)
{  
	
	long long unsigned int d;
 	long int common;
 	int count=0;
	if(num1>num2)
         {
            while(num1%num2 != 0)
             {
               d=(num1%num2);
               num1=num2;
               num2=d;
              }
         common=d;
            
          }
            
       else if(num1<num2)
         {
            while(num2%num1 != 0)
             {
               d=(num2%num1);
               num2=num1;
               num1=d;
              }
         common=d;
          }
       return common;
}

long int encryption(long int encrypt,long int product,long int plain)
{
 
 	int cipher=1;
 	long long unsigned int temp;
 	int arr=0,bits=0,i,j,binary[200]={0};
 	// converting encrypt value to binary
	
 	// Right to left binary method 
	
 	temp=encrypt;
 	while(temp != 1)                
 	{                              
		binary[arr]=temp%2;     
		temp=temp/2;
 	        arr++;
 	        bits++;
 	}
 	binary[arr]=1;      
 	  
	for(j=0;j<=bits;j++)
	{
		if(binary[j]==1)
		{
 	        	cipher=(cipher*plain)%product;
 	        	plain=(plain*plain)%product;
 	        }
 	        else if(binary[j]==0)
 	        {
 	        	plain=(plain*plain)%product;
 	        }
 	       

 	}
	return cipher;
	
 	   
}



long int decryption(long int decrypt,long int product,long int cipher)
{
	long int plain=1;
	long long unsigned int temp;
	int arr=0,bits=0,i,j,binary[200]={0};
 // converting encrypt value to binary

 // Right to left binary method.

     temp=decrypt;
      while(temp != 1)
       {                                            
          binary[arr]=temp%2;
          temp=temp/2;
          arr++;
          bits++;
        }
       binary[arr]=1;
      for(i=0;i<=bits;i++)
       {  
            if(binary[i]==1)
            {
               plain=(plain*cipher)%product;
               cipher=(cipher*cipher)%product;
             }
          else if(binary[i]==0)
            {
               cipher=(cipher*cipher)%product;
             }
        }
       printf("deciphered text : %ld\n",plain);
       printf("\n");
	return plain;

}

void file_encryption(struct encryption_value value)
{
	FILE *file_pointer,*file_pointer2;
	pthread_t encrypt[8];
	int i,j,count=0,counter,pass;
	char c;
	void * cipher;
	file_pointer = fopen("input.txt","r");
	file_pointer2 = fopen("cipher.txt","w");
	while(c != EOF )
	{
		c=getc(file_pointer);
		if( c == EOF )
			break;
		input_text[count] = (int)c;
		count++;
	}
	fclose(file_pointer);
	i=0;
	j=0;
	while(i < count)
	{
		for(counter=1;counter<2;counter++,i++)
		{
			
			if(i >= count)
			break;
			value.plain=input_text[i];
			pthread_create(&encrypt[counter],NULL,file_process,(void *)&value );
			pass=counter;
			
		}
		
		for(counter=1;counter<=pass;counter++)
		{
			pthread_join(encrypt[counter],&cipher);
			output_text[j]=(int)cipher;
			j++;
		}
	}
	for(i=0;i<j;i++)
	{
	fprintf(file_pointer2,"%d",(int)output_text[i]);
	fprintf(file_pointer2,"%c",' ');
	}
	fprintf(file_pointer2,"%d",-1);
	fclose(file_pointer2);
	printf("\n");
	
}

void *file_process(void *arg)
{
	struct encryption_value *value;
	long int out;
	value = (struct encryption_value *)arg;
	out=encryption(value->public_key,value->multiply,value->plain);
	return (void *)out;
}

void file_decryption(struct encryption_value value)
{
	FILE *file_pointer,*file_pointer2;
	int input[MAXIMUM],plain_text[MAXIMUM],j,count=0,count2=0,i=0,counter,pass;
	pthread_t decryption[8];
	void *plain;
	file_pointer=fopen("cipher.txt","r");
	while(1)
	{
		fscanf(file_pointer,"%d",&input[count]);
		count++;
		if(input[count-1] == -1)
			break;
		
		
	}	
	fclose(file_pointer);
	j=0;
	while(i < count-1)
	{
		for(counter=1;counter<2;counter++,i++)
		{
			
			if(i >= count)
			break;
			value.plain=input[i];
			pthread_create(&decryption[counter],NULL,file_deprocess,(void *)&value);

			pass=counter;
			
		}
		
		for(counter=1;counter<=pass;counter++,j++)
		{
			pthread_join(decryption[counter],&plain);
			plain_text[j]=(int)plain;
		}
	}
	
	printf("\n");
	file_pointer2=fopen("plain.txt","w");
	for(count2=0;count2<count-1;count2++)
	{
	fprintf(file_pointer2,"%c",(char)plain_text[count2]);
	printf("%c",(int)plain_text[count2]);
	}
	fclose(file_pointer2);
	printf("\n");
	return;
}

void *file_deprocess(void *arg)
{
	struct encryption_value *value;
	long int out;
	value = (struct encryption_value *)arg;
	out=decryption(value->private_key,value->multiply,value->plain);
	return (void *)out;
}
	
	
