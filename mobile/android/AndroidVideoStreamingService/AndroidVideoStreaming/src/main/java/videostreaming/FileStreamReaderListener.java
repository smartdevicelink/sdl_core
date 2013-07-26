package videostreaming;

/**
 * Created by Andrew Batutin on 7/24/13.
 */
public interface FileStreamReaderListener {

    public void fileReadWillBegin(VideoStreaming source);

    public void chunkIsReadFromFile(VideoStreaming source, byte[] chunk);

    public void fileReadEnded(VideoStreaming source);

}
