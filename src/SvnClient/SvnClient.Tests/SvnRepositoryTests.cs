using System;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using SvnClient.Backend;

namespace SvnClient.Tests
{
    [TestClass]
    public class SvnRepositoryTests
    {
        [TestMethod]
        public void SvnRepository_GetHistory_ReturnsData()
        {

            var connection = SvnConnection.OpenConnection("Connection", "https://svn.code.sf.net/p/passgenwin/code/");
            var repository = new SvnRepository();
            var svnCommitModels = repository.GetHistory(connection);
            Assert.IsTrue(svnCommitModels.Take(10).Count() > 0);
            Assert.IsTrue(svnCommitModels.Skip(10).Take(10).Count() > 0);
        }
    }
}
