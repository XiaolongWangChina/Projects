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

	public int run(String[] args) throws Exception {
		Configuration conf = getConf();
		JobConf conf = new JobConf();
		job.setJobName("ChainJob");
		job.setInputFormat(TextInputFormat.class);
		job.setOutputFormat(TextOutputFormat.class);

		FileInputFormat.setInputPaths(job, in);
		FileOutputFormat.setOutputPath(job, out);

		JobConf map1Conf = new JobConf(false);
		ChainMapper.addMapper(job, 
				Map1.class,
				LongWritable.class,
				Text.class,
				Text.class,
				Text.class,
				true,
				map1Conf);

		JobConf map2Conf = new JobConf(false);
		ChainMapper.addMapper(job, 
				Map2.class,
				LongWritable.class,
				Text.class,
				Text.class,
				Text.class,
				true,
				map2Conf);

		JobConf reduceConf = new JobConf(false);
		ChainReducer.setReducer(job,
				Reduce.class,
				LongWritable.class,
				Text.class,
				Text.class,
				Text.class,
				true,
				reduceConf);

		JobConf map3Conf = new JobConf(false);
		ChainMapper.addMapper(job, 
				Map3.class,
				LongWritable.class,
				Text.class,
				Text.class,
				Text.class,
				true,
				map3Conf);
		
		JobConf map4Conf = new JobConf(false);
		ChainMapper.addMapper(job, 
				Map4.class,
				LongWritable.class,
				Text.class,
				Text.class,
				Text.class,
				true,
				map4Conf);

		JobClient.runJob(job);
		return 0;
	}

	public static void main(String[] args) throws Exception {
		int res = ToolRunner.run(new Configuration(), new LinkMapReduce(), args);
		System.exit(res);
	}
}
