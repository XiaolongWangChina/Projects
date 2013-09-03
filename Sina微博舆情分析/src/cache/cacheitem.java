package cache;


public class cacheitem 
{
	String word1;//分别指代两个同义词
	String word2;
	int usedtimes;//记录被引用次数
	public cacheitem()
	{
		usedtimes=0;
		word1=word2=null;
	}
}
