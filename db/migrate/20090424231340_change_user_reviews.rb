class ChangeUserReviews < ActiveRecord::Migration
  def self.up
    remove_column :user_reviews, :profile_id
    add_column :user_reviews, :user_id, :integer
  end

  def self.down
    remove_column :user_reviews, :user_id
  end
end
