/*  printf.c  */
extern putch();

printf(fmt,firstparm)
  char *fmt;
{
	return uformat(putch,fmt,&firstparm);
}

/* uformat  -  K&R C standard output formatting  */

/************    Valid conversion specifications:

% {-} {nnn} {.} {mmm} {l} {d,o,x,u,c,s,e,f,g}
 
  - = left-justify instead of right-justify
nnn = field width (0 start means pad with '0' instead of spaces)
mmm = precision (maximum # chars or # of digits to right of '.')
       (default precision = 6)
  l = long, not int.
  d = decimal (int or long int)
  o = unsigned octal (no leading zero)
  x = unsigned hex (no leading 0x)
  u = unsigned decimal (int or long int)
  c = single character
  s = string
  e = [-]x.yyyyyyE[+/-]ee    where precison = # y's
  f = [-]xxx.yyyyyy          where precision = # y's
       (if precision==0, no decimal point is printed)
  g = use %e or %f, whichever is shorter. (%f if equal)

*********************************************/
extern char	*ultoa(),*ltoa(),*ecvt(),*fcvt();

int uformat( put, fmt, args )
   /** Returns # of characters printed **/
  int (*put)(), *args;  char *fmt;
{
	unsigned char c,left_justify,left_zpad,long_prefix,
		conv[33],*p,*expp,expsign,*s;
	double dblval;  long int lival;
	int dec,point,sign,pad1,pad2,pad3,pad4,cnt1,cnt2,explen,
		retval,width,excess,pad,precision,exp,radix;

	retval = 0;
	while(c=*fmt++)
	{
		if(c!='%' || *fmt=='%')
		{
			if(c=='%')  fmt++;
			(*put)(c);
			retval++;
		}
		else	/* c==%, not %%  */
		{
			pad1=pad2=pad3=pad4=sign=point=explen=cnt1=cnt2=0;
			radix = 16;  excess = 0;  precision = -3;
			if(left_justify = (*fmt=='-'))  fmt++;
			left_zpad = (*fmt=='0');
			if((width = nextnum(&fmt)) < 0) width = 0;
			if(*fmt == '.')
				{fmt++;  precision = nextnum(&fmt);}
			if(long_prefix = (*fmt=='l'))  fmt++;

switch(c=*fmt++)
{
	case 'e':  precision++;
	case 'g':
	case 'f':
		dblval = *((double*)args)++;
		if(precision<0)  precision += 9;    /* 6 or 7 */
		if(c=='f')
			p = fcvt(dblval, precision, &dec, &sign);
		else
		{
			p = ecvt(dblval, precision, &dec, &sign);
			exp = dec-1;
			if(exp<0)
				{exp = -exp;  expsign = '-';}
			else
				expsign = '+';
			conv[0] = '0';
			ultoa( (long)exp, conv+1, 10);
			expp = (conv[2]=='\0') ? conv : conv+1;
			explen = strlen(expp)+2;
		}
		if(sign) sign = 1;
		if(c=='g')
		{
			if(dec>=-3 && dec<=precision+5)
				explen = 0;
			s = p+strlen(p);
			if(s!=p)  while(--s!=p)  if(*s=='0')  *s='\0';
		}
		cnt1 = 1;
		if(!explen)
		{
			if(dec<=0)
			{
				pad2 = 1;
				pad3 = -dec;
			}
			cnt1 = (dec>0)? dec : 0;
		}
		cnt2 = strlen(p) - cnt1;
		if(pad3+cnt2)   point = 1;
		pad = width - sign - pad2
			- cnt1 - point - pad3
			- cnt2 - explen;
		if(pad<0)
		{
			pad = 0;
			excess = -pad;
		}
		if(left_justify)
			pad4 = pad;
		else
			pad1 = pad;
		retval += width + excess;
		break;

	case 'd':
	case 'o': radix -= 2;	/*  8 */
	case 'u': radix -= 6;	/* 10 */
	case 'x':	/* 16 */
		
		if(c=='d')
		{
			lival = long_prefix ?  *((long*)args)++  :  *args++;
			p = ltoa( lival,conv,10 );  /* signed conversion */
		}
		else
		{
			lival = long_prefix ? *((unsigned long*)args)++  :
				*((unsigned int*)args)++;
			p = ultoa(lival,conv,radix);  /* unsigned conversion */
		}
		if(precision <= 0)   precision = 1;
		cnt2 = strlen(p);
		if(*p=='-')
			{cnt2--;  cnt1=1;}
		if((pad3=precision-cnt1-cnt2) < 0)   pad3 = 0;
		if( (pad=width-cnt1-cnt2-pad3) < 0 )
		{
			excess = -pad;
			pad = 0;
		}
		if(left_justify)
			pad4 = pad;
		else if(!left_zpad)
			pad1 = pad;
		else
			pad3 += pad;
		retval += width + excess;
		break;

case 's': p = *((char**)args++);
case 'c': if(c=='c')
			{
				conv[1] = *args++;
				conv[2] = '\0';
				p = conv+1;
			}
		cnt1 = strlen(p);
		if((precision>0)&&(precision<cnt1))  cnt1 = precision;
		if( (pad=width-cnt1) < 0 )
			{excess = -pad;  pad = 0;}
		if(left_justify)
			pad4 = pad;
		else
			pad1 = pad;
		retval += width + excess;
		break;

		default: (*put)(c); retval++;  break;
}

			while(pad1--)  (*put)(' ');
			if(sign)       (*put)('-');
			while(pad2--)  (*put)('0');
			while(cnt1--)  (*put)(*p++);
			if(point)      (*put)('.');
			while(pad3--)  (*put)('0');
			while(cnt2--)  (*put)(*p++);
			if(explen--)
			{
				(*put)('E');
				(*put)(expsign);
				while(--explen>=0) (*put)(*expp++);
			}
			while(pad4--)  (*put)(' ');
		}
	}
	return retval;
}

static int nextnum(f)
	unsigned char **f;
{
	register int r;   char invalid,ch;
	invalid = '\1';
	for(r=0; (ch=**f, ch>='0' && ch<='9'); invalid = '\0')
		{r = r*10 + (ch-'0');    (*f)++;}
	return invalid ? -3 : r;
}
                                                                                                                                                                                                                                                                                                                                                                               