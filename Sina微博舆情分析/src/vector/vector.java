package vector;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import similarword.similarword;

import dataStructure.SinaWeibo;
import dataStructure.TextSeg;
import ICTCLAS.I3S.AC.ICTCLAS50;
import weibo.Weibo;

public class vector {
	private List<Byte[]> vectorlist = new ArrayList<Byte[]>();
	private static int vectordivision =500;
	public List<Byte[]> getVectorlist(){
		this.reduceVector();
		return this.vectorlist;
	}

	public void reduceVector(){
		Weibo weibo=new Weibo();
		weibo.init();
		ArrayList<SinaWeibo> myweibo=new ArrayList<SinaWeibo>();
		myweibo=weibo.getSinaWeibo();
		if(myweibo==null)
			System.out.println("!!!");
		Iterator<SinaWeibo> it=myweibo.iterator();
		HashMap<String,Integer> hsMap=new HashMap<String,Integer>();
		hsMap=weibo.gethsMap();
		/*while(it.hasNext()){
		  SinaWeibo itn=it.next();
		//System.out.println(itn.getDocno());
		//System.out.println(itn.getText());
		//System.out.println(itn.getUserid());
		Iterator<TextSeg> segit=itn.getTextSeg().iterator();
		while(segit.hasNext()){
		TextSeg ntex=segit.next();
		String type=ntex.getTextSegPOS();
		String txt=ntex.getTextSeg();
		//if(type.substring(0, 1).equals("n")||type.substring(0,1).equals("a")){
		if(hsMap.containsKey(txt)){
		Integer n=hsMap.get(txt);
		hsMap.put(txt, n+1);
		}
		else
		hsMap.put(txt, 1);
		//}
		}
		}*/
		System.out.println("分词统计完毕");
		Map.Entry[] result=getSortedHashtableByValue(hsMap);
		/*
		   for (int i = 0; i <result.length; i++) {
		   Map.Entry entry =result[i];
		   System.out.println(entry.getKey());
		   System.out.println(entry.getValue());
		   }
		 */
		similarword similar=new similarword();
		String property = null;
		int rLength=result.length;
		System.out.println(result.length);
		hsMap.clear();
		int mark[] =new int[this.vectordivision];
		for(int i=0;i<this.vectordivision;i++){
			mark[i]=i;
			hsMap.put((String)result[i].getKey(),i);
		}
		System.out.println("mark完毕");
		Map.Entry[] myresult=new Map.Entry[this.vectordivision];
		HashMap<Integer,Integer> hsindex=new HashMap<Integer,Integer>();
		for(int i=0;i<this.vectordivision;i++){
			myresult[i]=result[i];
		}
		result=null;
		for(int i=0;i<this.vectordivision;i++)
		{
			if(mark[i]==i)
			{
				String word1=(String)myresult[i].getKey();
				for(int j=i+1;j<this.vectordivision;j++)
				{
					if(mark[j]==j)
					{
						String word2 = (String)myresult[j].getKey();
						//System.out.println(time);
						if(similar.finalverdict(word1,word2,property)==1)
						{
							mark[j]=i;
							//result[i].setValue((Integer)result[i].getValue()+(Integer)result[j].getValue());
						}
						//time++;
					}
					System.out.println(i+"||"+j);
				}
			}
		}
		System.out.println("mark第二阶段");

		int num=0;

		//ArrayList<String> vecStr=new ArrayList<String>();
		for(int i=0;i<this.vectordivision;i++){
			//myresult[i]=result[i];
			if(mark[i]==i){
				hsindex.put(i,num);
				num++;
			}
		}
		System.out.println("清理完毕");
		/* System.out.println("分词统计完毕2");
		   for (int i = 0; i <rLength; i++) {
		   if(!isMemberOf(i,mark)){
		   Map.Entry entry =result[i];
		//System.out.println(entry.getKey());
		//System.out.println(entry.getValue());
		hsMap.put((String)entry.getKey(),(Integer)entry.getValue());
		}
		}
		result=getSortedHashtableByValue(hsMap);
		for (int i = 0; i <result.length; i++) {
		Map.Entry entry =result[i];
		System.out.println(entry.getKey());
		System.out.println(entry.getValue());
		}
		String[] vectorResult = new String[this.vectordivision];
		for(int i=0;i<this.vectordivision;i++)
		{
		vectorResult[i]=(String)result[i].getKey();
		System.out.print(vectorResult[i]+" ");		
		} */
		File fileatr=new File( "data\\attribute.txt"); 
		try {
			FileWriter writer=new FileWriter(fileatr,true);
			StringBuffer buff = new StringBuffer();
			for (int i = 0; i < this.vectordivision; i++)
			{
				if(mark[i]==i){    
					buff.append("attribute"+i+"  "+myresult[i].getKey()+" ");
					System.out.println((String)myresult[i].getKey()+" 文本");
					System.out.println("打印");
				}
			}
			buff.append(System.getProperty("line.separator"));
			byte s[]= buff.toString().getBytes("UTF-8");
			writer.write(buff.toString(),0,buff.toString().length());
			writer.flush(); 
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		File file=new File( "data\\500u.txt"); 
		try {
			FileWriter writer=new FileWriter(file,true);
			StringBuffer buff = new StringBuffer();
			for (int i = 0; i < this.vectordivision; i++)
			{
				if(mark[i]==i){    
					//buff.append(myresult[i].getKey()+" ");
					buff.append("attribute"+i+" ");
					System.out.println((String)myresult[i].getKey()+" 文本");
					System.out.println("打印");
				}
			}
			buff.append(System.getProperty("line.separator"));
			byte s[]= buff.toString().getBytes("UTF-8");
			writer.write(buff.toString(),0,buff.toString().length());
			writer.flush(); 
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}


		int count=0;
		Iterator<SinaWeibo> it2=myweibo.iterator();
		while(it2.hasNext()){
			Byte[] byteTemp=new Byte[num];
			for(int i=0;i<byteTemp.length;i++) byteTemp[i]=0;
			SinaWeibo itn=it2.next();
			Iterator<TextSeg> segit=itn.getTextSeg().iterator();
			while(segit.hasNext()){
				TextSeg ntex=segit.next();
				//String type=ntex.getTextSegPOS();
				String txt=ntex.getTextSeg();
				if(hsMap.containsKey(txt)){
					int i=hsMap.get(txt);
					byteTemp[hsindex.get(mark[i])]=1;
				}
			}
			//this.vectorlist.add(byteTemp);	

			//		File file=new File( "data\\vectorlist.txt"); 
			try {
				FileWriter writer=new FileWriter(file,true);
				StringBuffer buff = new StringBuffer();
				for (int i = 0; i < byteTemp.length; i++)
				{
					buff.append(byteTemp[i] + " ");
				}
				buff.append(System.getProperty("line.separator"));
				byte s[]= buff.toString().getBytes("UTF-8");
				writer.write(buff.toString(),0,buff.toString().length());
				count++;
				//writer.write("\n");
				writer.flush(); 
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if(count%100==0){
				System.out.println(count);
			}

		}


	}

	public static Map.Entry[] getSortedHashtableByValue(Map h) {
		Set set = h.entrySet();
		Map.Entry[] entries = (Map.Entry[]) set.toArray(new Map.Entry[set.size()]);
		h.clear();
		Arrays.sort(entries, new Comparator() {
				public int compare(Object arg0, Object arg1) {
				Double key1 = Double.valueOf(((Map.Entry) arg0).getValue()
					.toString());
				Double key2 = Double.valueOf(((Map.Entry) arg1).getValue()
					.toString());
				return -key1.compareTo(key2);
				}
				});
		return entries;
	}	
}
