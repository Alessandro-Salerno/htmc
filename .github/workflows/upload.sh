git config --local user.email "41898282+github-actions[bot]@users.noreply.github.com"
git config --local user.name "github-actions[bot]"
git checkout --orphan latest-linux-bundle
git rm -rf .github
git rm -rf *.md
git rm -rf .gitattributes
git rm -rf .gitignore
git rm -rf .clang-format
git rm -rf src
git add bin/*
git add include/*
git add examples/*
git commit -m "new bundle"
git push --set-upstream origin latest-linux-bundle
