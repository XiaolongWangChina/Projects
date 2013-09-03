package cache;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class cache
{
	cacheitem []cache;//这是cache的主体
	int used;//已经使用的cache
	int cachesize=5;//cache的大小
	String cachefilename="cacherecord.txt";//cache的文件记录,每对词为一行，两个词之间隔着一个$，再隔着个%，最后为引用次数，如悲伤$伤心%100

	public cache() //初始化，读取文件上的记录
	{
		used=0;                                       //System.out.println("  Initialization!!");
		cache=new cacheitem[cachesize];//
		for(int i=0;i<cachesize;i++)cache[i]=new cacheitem();
		File file = new File(cachefilename);
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new FileReader(file));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		String temp=null;
		int middle=0,back=0;
		try {
			while((temp=reader.readLine())!=null) //读入记录
			{                                                  
				middle=temp.indexOf('$');//从读入的句子中截取内容
				back=temp.indexOf('%');                         //System.out.println(temp+" "+String.valueOf(middle)+" "+String.valueOf(back));
				cache[used].word1=temp.substring(0, middle);
				cache[used].word2=temp.substring(middle+1, back);
				cache[used].usedtimes=Integer.parseInt(temp.substring(back+1,temp.length()));//对于以前的记录，默认为1.
				//System.out.println(cache[used].word1+" "+cache[used].word2+" "+temp.substring(back+1,temp.length()));
				used++;//used从1开始才是非空!!!!!!!!!
			}
		} catch (NumberFormatException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			reader.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}//cache()

	public int getmin()//返回使用次数最少的cacheitem
	{                                             //System.out.println("  getmin!!!");
		int pos=0;
		if(cache[pos].usedtimes==0){used++;return pos;}
		for(int i=1;i<cachesize;i++)
		{
			if(cache[i].usedtimes<cache[pos].usedtimes)
			{
				pos=i;
				if(cache[pos].usedtimes==0){used++;return pos;}//min取0说明找到的是新的item，因此计数加一
			}
		}
		return pos;
	}//getmin

	public void rewrite()//在不用后将cache写会文件保存
	{                                               //System.out.println("  rewrite!!!");
		File file = new File(cachefilename);
		BufferedWriter writer = null;
		try {
			writer = new BufferedWriter(new FileWriter(file));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		for(int i=0;i<used;i++)
		{
			try {
				writer.write(cache[i].word1+"$"+cache[i].word2+"%"+String.valueOf(cache[i].usedtimes)+"\r\n");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		try {
			writer.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		try {
			writer.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}//rewrite

	public int VerdictInCache(String word1,String word2)//cache里有这对词则返回1，无则返回0
	{                                    // System.out.println("  VerdictInCache!!");
		for(int i=0;i<used;i++)
		{                           //System.out.println(cache[i].word1+" "+cache[i].word2);
			if(cache[i].word1.equals(word1)&&cache[i].word2.equals(word2))
			{
				cache[i].usedtimes++;//System.out.println("cache里有!!");
				return 1;
			}
			if(cache[i].word1.equals(word2)&&cache[i].word2.equals(word1))
			{
				cache[i].usedtimes++;
				//System.out.println("cache里有!!");
				return 1;
			}
		} //System.out.println("cache里没有");
		return 0;
	}//VerdictInCache

	public void AddCacheitem(String word1,String word2)//增加一个新条目
	{                            // System.out.println("  AddCacheitem!!");
		int pos=getmin();
		cache[pos].word1=word1;
		cache[pos].word2=word2;
		cache[pos].usedtimes=1;//因为是新加的
	} //addcacheitem

}
