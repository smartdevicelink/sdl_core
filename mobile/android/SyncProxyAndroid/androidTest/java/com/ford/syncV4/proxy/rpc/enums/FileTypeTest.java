package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for FileType enum.
 *
 * Created by enikolsky on 2014-02-11.
 */
public class FileTypeTest extends TestCase {
    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(FileType.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        FileType.GRAPHIC_BMP, FileType.GRAPHIC_JPEG,
                        FileType.GRAPHIC_PNG, FileType.AUDIO_WAVE,
                        FileType.AUDIO_MP3, FileType.AUDIO_AAC, FileType.BINARY,
                        FileType.JSON));
    }

    public void testValueForStringGRAPHIC_BMPShouldReturnCorrectValue() {
        assertThat(FileType.valueForString("GRAPHIC_BMP"),
                is(FileType.GRAPHIC_BMP));
    }

    public void testValueForStringGRAPHIC_JPEGShouldReturnCorrectValue() {
        assertThat(FileType.valueForString("GRAPHIC_JPEG"),
                is(FileType.GRAPHIC_JPEG));
    }

    public void testValueForStringGRAPHIC_PNGShouldReturnCorrectValue() {
        assertThat(FileType.valueForString("GRAPHIC_PNG"),
                is(FileType.GRAPHIC_PNG));
    }

    public void testValueForStringAUDIO_WAVEShouldReturnCorrectValue() {
        assertThat(FileType.valueForString("AUDIO_WAVE"),
                is(FileType.AUDIO_WAVE));
    }

    public void testValueForStringAUDIO_MP3ShouldReturnCorrectValue() {
        assertThat(FileType.valueForString("AUDIO_MP3"),
                is(FileType.AUDIO_MP3));
    }

    public void testValueForStringAUDIO_AACShouldReturnCorrectValue() {
        assertThat(FileType.valueForString("AUDIO_AAC"),
                is(FileType.AUDIO_AAC));
    }

    public void testValueForStringBINARYShouldReturnCorrectValue() {
        assertThat(FileType.valueForString("BINARY"), is(FileType.BINARY));
    }

    public void testValueForStringJSONShouldReturnCorrectValue() {
        assertThat(FileType.valueForString("JSON"), is(FileType.JSON));
    }

    public void testGRAPHIC_BMPToStringShouldReturnCorrectValue() {
        assertThat(FileType.GRAPHIC_BMP.toString(), is("GRAPHIC_BMP"));
    }

    public void testGRAPHIC_JPEGToStringShouldReturnCorrectValue() {
        assertThat(FileType.GRAPHIC_JPEG.toString(), is("GRAPHIC_JPEG"));
    }

    public void testGRAPHIC_PNGToStringShouldReturnCorrectValue() {
        assertThat(FileType.GRAPHIC_PNG.toString(), is("GRAPHIC_PNG"));
    }

    public void testAUDIO_WAVEToStringShouldReturnCorrectValue() {
        assertThat(FileType.AUDIO_WAVE.toString(), is("AUDIO_WAVE"));
    }

    public void testAUDIO_MP3ToStringShouldReturnCorrectValue() {
        assertThat(FileType.AUDIO_MP3.toString(), is("AUDIO_MP3"));
    }

    public void testAUDIO_AACToStringShouldReturnCorrectValue() {
        assertThat(FileType.AUDIO_AAC.toString(), is("AUDIO_AAC"));
    }

    public void testBINARYToStringShouldReturnCorrectValue() {
        assertThat(FileType.BINARY.toString(), is("BINARY"));
    }

    public void testJSONToStringShouldReturnCorrectValue() {
        assertThat(FileType.JSON.toString(), is("JSON"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            FileType.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
