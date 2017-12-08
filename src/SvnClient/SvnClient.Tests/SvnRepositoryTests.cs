using System;
using System.Linq;
using NUnit.Framework;
using SvnClient.Backend;
using SvnClient.Backend.Models;

namespace SvnClient.Tests
{
    [TestFixture]
    public class SvnRepositoryTests
    {
        [Test]
        public void SvnRepository_GetHistory_ReturnsData()
        {

            var connection = SvnConnection.OpenConnection("Connection", "https://svn.code.sf.net/p/heifaratest/code-0/");
            var repository = new SvnRepository();
            var svnCommitModels = repository.GetHistory(connection).ToList();

            repository.GetFileDiff(connection, svnCommitModels.First(), svnCommitModels.ElementAt(1), svnCommitModels.First().Changes.First(c => c.Type == SvnChangeType.Modify));
        }
    }
}
