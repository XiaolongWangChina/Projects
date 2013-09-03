package jobcontrol;

import java.io.IOException;
import java.util.Iterator;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapred.FileInputFormat;
import org.apache.hadoop.mapred.FileOutputFormat;
import org.apache.hadoop.mapred.JobClient;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.KeyValueTextInputFormat;
import org.apache.hadoop.mapred.MapReduceBase;
import org.apache.hadoop.mapred.Mapper;
import org.apache.hadoop.mapred.OutputCollector;
import org.apache.hadoop.mapred.Reducer;
import org.apache.hadoop.mapred.Reporter;
import org.apache.hadoop.mapred.TextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class LinkMapReduce extends Configured implements Tool {

	public static class Map1 extends MapReduceBase
		implements Mapper<Text, Text, Text, Text> {
			public void map(Text key, Text value,
					OutputCollector<Text, Text> output,
					Reporter reporter) throws IOException {
				output.collect(value, key);
			}
		}

	public static class Reduce1 extends MapReduceBase
		implements Reducer<Text, Text, Text, IntWritable> {
			public void reduce(Text key, Iterator<Text> values, 
					OutputCollector<Text, IntWritable> output,
					Reporter reporter) throws IOException {
				int count = 0;
				while (values.hasNext()) {
					values.next();
					count++;
				}
				output.collect(key, new IntWritable(count));
			}
		}

	public static class Map2 extends MapReduceBase
		implements Mapper<Text, Text, IntWritable, IntWritable> {
			private final static IntWritable uno = new IntWritable(1);
			private IntWritable citationCount = new IntWritable();
			public void map(Text key, Text value,
					OutputCollector<IntWritable, IntWritable> output,
					Reporter reporter) throws IOException {
				citationCount.set(Integer.parseInt(value.toString()));
				output.collect(citationCount, uno);
			}
		}

	public static class Reduce2 extends MapReduceBase
		implements Reducer<IntWritable, IntWritable, IntWritable, IntWritable> {
			public void reduce(IntWritable key, Iterator<IntWritable> values, 
					OutputCollector<IntWritable, IntWritable> output,
					Reporter reporter) throws IOException {
				int count = 0;
				while (values.hasNext()) {
					count += values.next().get();
				}
				output.collect(key, new IntWritable(count));
			}
		}

	public int run(String[] args) throws Exception {
		// MapReduce1
		Configuration conf = getConf();
		JobConf job = new JobConf(conf, LinkMapReduce.class);

		Path in = new Path(args[0]);
		Path out = new Path("temp");
		FileInputFormat.setInputPaths(job, in);
		FileOutputFormat.setOutputPath(job, out);

		job.setJobName("MapReduce1");
		job.setMapperClass(Map1.class);
		job.setReducerClass(Reduce1.class);

		job.setInputFormat(KeyValueTextInputFormat.class);
		job.set("key.value.separator.in.input.line", ",");

		job.setOutputFormat(TextOutputFormat.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);

		JobClient.runJob(job);

		// MapReduce2
		Configuration conf2 = getConf();
		JobConf job2 = new JobConf(conf2, LinkMapReduce.class);
		
		Path in2 = new Path("temp");
		Path out2 = new Path(args[1]);
		FileInputFormat.setInputPaths(job2, in2);
		FileOutputFormat.setOutputPath(job2, out2);

		job2.setJobName("MapReduce2");
		job2.setMapperClass(Map2.class);
		job2.setReducerClass(Reduce2.class);

		job2.setInputFormat(KeyValueTextInputFormat.class);
		job2.set("key.value.separator.in.input.line", "\t");

		job2.setOutputFormat(TextOutputFormat.class);
		job2.setOutputKeyClass(IntWritable.class);
		job2.setOutputValueClass(IntWritable.class);

		JobClient.runJob(job2);

		return 0;
	}

	public static void main(String[] args) throws Exception {
		int res = ToolRunner.run(new Configuration(), new LinkMapReduce(), args);
		System.exit(res);
	}
}
