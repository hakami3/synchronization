package Prosumer;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.ArrayBlockingQueue;  
import java.util.concurrent.BlockingQueue;  
 
 
public class Prosumer {  
    private static BlockingQueue<Integer> queue = new ArrayBlockingQueue<Integer>(250);
    private volatile static int count=1;
    private static int p;
    private static int use;
    private static final int MAX =250;
      
	public static void main(String[] args) throws InterruptedException  { 
		System.out.println("입력 방법: 생산자 개수, 소비자 개수, 소비자당 소비 개수");
        Scanner s = new Scanner(System.in);
        p =s.nextInt();
        int c =s.nextInt();
        use=s.nextInt();
        s.close();
        Producer pp[]=new Producer[p];
        Consumer cc[]=new Consumer[c];
        ArrayList<Producer> threadList1= new ArrayList<Producer>();
        ArrayList<Consumer> threadList2= new ArrayList<Consumer>();
        for(int a=0;a<p;a++) {
        	pp[a]=new Producer(a,queue);
        }
        
        for(int a=0;a<c;a++) {
        	cc[a]=new Consumer(a,queue);	
        }
        
        for(int b=0; b<p; b++) {
        	pp[b].start();	
			threadList1.add(pp[b]);	
        	pp[b].join();
        }
        
        for(int i=0;i<threadList1.size();i++) {
        	try {
        		threadList1.get(i).join();        		
        	}catch(InterruptedException e) {
        		e.printStackTrace();
        	}
        }
        
		for(int i=0; i<c;i++) {
			cc[i].start();
			threadList2.add(cc[i]);
			cc[i].join();
		}
        for(int i=0;i<threadList2.size();i++) {
        	try {
        		threadList2.get(i).join();        		
        	}catch(InterruptedException e) {
        		e.printStackTrace();
        	}
        }

    }  
    static class Producer extends Thread {  
        private int name;
        private BlockingQueue<Integer> queue;  
          
        public Producer(int name,BlockingQueue<Integer> queue) {  
            this.queue = queue;  
            this.name= name;   
        }  
          
        public void run() {  
            while(!Thread.currentThread().isInterrupted()&&count<MAX) {
                try {  
                    Thread.sleep(10);
                    queue.put(count);
                    System.err.println("Producer " + name + ": " + count);
                    if(MAX/p==count)
                    {
                    	Thread.currentThread().interrupt();
                    }    
                    count++;
                    
                } catch (InterruptedException e) {  
                    e.printStackTrace();  
                }  
            
            }
        }  
}  
      
      
    static class Consumer extends Thread {  
        private BlockingQueue<Integer> queue;  
        private int name;  
        public Consumer(int name, BlockingQueue<Integer> queue) {  
            this.name = name;  
            this.queue = queue;  
        }  

          
        public void run() {  
            while (!Thread.currentThread().isInterrupted()) {  
                try {  
                    // queue에 data가 없으면 알아서 wait하고 있다.  
                    Integer index = queue.take(); 
                    if(index>99&index<200)
                    	index-=100;
                    else if(index>199)
                    	index-=200;
                    if((index==use))
                    {
                    	Thread.currentThread().interrupt();
                    }   
                    System.err.println("Consumer " + name + ": " + index); 
                } catch (InterruptedException e) {  
                    e.printStackTrace();  
                }  
            }  
        }  
    }  
}  