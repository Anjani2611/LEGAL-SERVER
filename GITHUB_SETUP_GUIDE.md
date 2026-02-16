# 📦 GitHub Repository Setup Guide

## Files You Need to Upload

You've downloaded 4 files:
1. **README.md** - Main project documentation
2. **LICENSE** - MIT License
3. **CONTRIBUTING.md** - Contribution guidelines
4. **ci.yml** - GitHub Actions workflow

---

## 🚀 Step-by-Step Instructions

### Option A: Manual Upload via GitHub Website (Easiest - 5 mins)

#### Step 1: Create GitHub Repository
1. Go to https://github.com/new
2. Fill in:
   - **Repository name**: `legal-server`
   - **Description**: `High-performance C++ backend for legal document simplification with ML integration`
   - **Visibility**: Public or Private (your choice)
   - ☐ **DO NOT** check "Add a README file"
   - ☐ **DO NOT** add .gitignore
   - ☐ **DO NOT** choose a license
3. Click **"Create repository"**

#### Step 2: Upload Files via Web Interface
1. On the empty repository page, click **"uploading an existing file"** link
2. Drag and drop these 3 files:
   - `README.md`
   - `LICENSE`
   - `CONTRIBUTING.md`
3. Add commit message: `docs: Add project documentation and license`
4. Click **"Commit changes"**

#### Step 3: Create GitHub Actions Workflow
1. In your repository, click **"Actions"** tab
2. Click **"Skip this and set up a workflow yourself"**
3. GitHub will create `.github/workflows/main.yml`
4. Delete the default content
5. Copy-paste the content from `ci.yml` file
6. Rename file from `main.yml` to `ci.yml`
7. Click **"Commit changes"**

#### Step 4: Connect Local Repository
```bash
cd /d/dev/legal-server

# Add GitHub remote (replace YOUR_USERNAME with your actual username)
git remote add origin https://github.com/YOUR_USERNAME/legal-server.git

# Pull the files you uploaded
git pull origin master --allow-unrelated-histories

# Push your local code
git push -u origin master
```

---

### Option B: Command Line Upload (For Advanced Users - 3 mins)

#### Step 1: Place Files in Your Local Repository
```bash
cd /d/dev/legal-server

# Copy the downloaded files to your repository
cp ~/Downloads/README.md .
cp ~/Downloads/LICENSE .
cp ~/Downloads/CONTRIBUTING.md .

# Create .github/workflows directory
mkdir -p .github/workflows
cp ~/Downloads/ci.yml .github/workflows/
```

#### Step 2: Create GitHub Repository
1. Go to https://github.com/new
2. Repository name: `legal-server`
3. **DO NOT** initialize with README, .gitignore, or license
4. Click **"Create repository"**

#### Step 3: Stage, Commit, and Push
```bash
cd /d/dev/legal-server

# Stage new files
git add README.md LICENSE CONTRIBUTING.md .github/

# Commit
git commit -m "docs: Add project documentation, license, and CI workflow"

# Add remote (replace YOUR_USERNAME)
git remote add origin https://github.com/YOUR_USERNAME/legal-server.git

# Push
git push -u origin master
```

---

## ✅ Verification Checklist

After upload, visit your repository at `https://github.com/YOUR_USERNAME/legal-server`:

- [ ] README.md displays on the repository homepage
- [ ] LICENSE file is visible in file list
- [ ] CONTRIBUTING.md is accessible
- [ ] GitHub Actions tab shows the CI workflow
- [ ] All your local folders (src/, include/, etc.) are visible
- [ ] Repository has a nice description
- [ ] Green checkmark appears on commits (CI running)

---

## 🔐 Authentication Help

### If Git Push Asks for Password:

**You need a Personal Access Token (PAT), not your password!**

#### Create a PAT:
1. GitHub → Your profile (top-right) → **Settings**
2. Scroll down → **Developer settings** (left sidebar)
3. **Personal access tokens** → **Tokens (classic)**
4. Click **"Generate new token"** → **"Generate new token (classic)"**
5. Configure:
   - **Note**: `legal-server repository access`
   - **Expiration**: 90 days (or custom)
   - **Scopes**: Check ☑ **repo** (Full control of private repositories)
6. Click **"Generate token"**
7. **COPY THE TOKEN NOW** (you won't see it again!)
   - Example: `ghp_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`

#### Use the Token:
```bash
# When git push asks for password, paste the token
git push -u origin master
Username: YOUR_USERNAME
Password: [paste token here, not your password]
```

---

## 📝 Next Steps After Upload

Once your repository is live on GitHub:

1. **Update README.md** on GitHub:
   - Replace `YOUR_USERNAME` with your actual username
   - Update any placeholder text

2. **Share your repository**:
   - Add to your resume/portfolio
   - Share on LinkedIn
   - Contribute to open source

3. **Start implementing Week 1**:
   - HTTP Server with Boost.Asio
   - Configuration management
   - Logging system

4. **Set up CI/CD**:
   - Watch GitHub Actions run on every push
   - Add tests as you implement features

---

## ❓ Troubleshooting

### Error: "Repository not found"
**Solution**: Check your remote URL
```bash
git remote -v
# Should show: https://github.com/YOUR_USERNAME/legal-server.git
```

### Error: "Authentication failed"
**Solution**: Use Personal Access Token instead of password

### Error: "refusing to merge unrelated histories"
**Solution**: Use `--allow-unrelated-histories` flag
```bash
git pull origin master --allow-unrelated-histories
```

### Can't create .github folder on Windows?
**Solution**: Use Git Bash or WSL, not Windows Explorer

---

## 🎯 Quick Commands Reference

```bash
# Check repository status
git status

# View commit history
git log --oneline

# Check remote connection
git remote -v

# Pull latest changes
git pull origin master

# Push new commits
git push origin master

# View differences
git diff
```

---

## 📧 Need Help?

If you encounter any issues:
1. Copy the exact error message
2. Share your git status output: `git status`
3. Share your remote info: `git remote -v`
4. Let me know which step failed

---

**Happy Coding!** 🚀
