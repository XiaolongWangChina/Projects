package dataStructure;

public class TextSeg {
	private String textSeg=null; //´Ê
	private String textSegPOS=null;//´ÊÐÔ

	public TextSeg(String textSeg,String textSegPOS)
	{
		this.textSeg=textSeg;
		this.textSegPOS=textSegPOS;
	}


	public String getTextSeg() {
		return textSeg;
	}


	public void setTextSeg(String textSeg) {
		this.textSeg = textSeg;
	}


	public String getTextSegPOS() {
		return textSegPOS;
	}


	public void setTextSegPOS(String textSegPOS) {
		this.textSegPOS = textSegPOS;
	}




}
