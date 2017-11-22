using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using DinkToPdf;

namespace InformationSystem.BL.Services
{
    public class PdfService
    {
        public byte[] ConvertHtmlToPdf(string html)
        {
            var converter = new BasicConverter(new PdfTools());
            var document = new HtmlToPdfDocument
            {
                GlobalSettings =
                {
                    ColorMode = ColorMode.Color,
                    Orientation = Orientation.Landscape,
                    PaperSize = PaperKind.A4Plus,
                },
                Objects =
                {
                    new ObjectSettings()
                    {
                        PagesCount = true,
                        HtmlContent = html,
                        WebSettings = {DefaultEncoding = "utf-8" }
                    },
                }
            };

            return converter.Convert(document);

        }
    }
}